/*
 * QDiscord - An unofficial C++ and Qt wrapper for the Discord API.
 * Copyright (C) 2016-2017 george99g
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.     If not, see <http://www.gnu.org/licenses/>.
 */

#include "qdiscordrestcomponent.hpp"
#include <QBuffer>

QDiscordRestComponent::QDiscordRestComponent(QObject *parent)
    : QObject(parent)
{
    _authentication = "";
    _self = QSharedPointer<QDiscordUser>();
    _loggedIn = false;

#ifdef QDISCORD_LIBRARY_DEBUG
    qDebug() << this << "constructed";
#endif
}

QDiscordRestComponent::~QDiscordRestComponent()
{
#ifdef QDISCORD_LIBRARY_DEBUG
    qDebug() << this << "destroyed";
#endif
}

void QDiscordRestComponent::login(const QString &token, const QDiscordTokenType &tokenType)
{
#ifdef QDISCORD_LIBRARY_DEBUG
    qDebug() << this << "verifying token";
#endif

    if (!_authentication.isEmpty())
    {
#ifdef QDISCORD_LIBRARY_DEBUG
        qDebug() << this << "attempted to verify a token while one is already stored";
#endif

        return;
    }

    _authentication = QDiscordUtilities::convertTokenToType(token, tokenType);
    doRequest(QDiscordRoutes::Self::login(),
    [this, token, tokenType](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            _authentication.clear();
            _loggedIn = false;
            emit loginFailed(reply->error());
            return;
        }

        _loggedIn = true;
        emit tokenVerified(token, tokenType);
    });
}

void QDiscordRestComponent::sendMessage(const QString &content,
                                        QSharedPointer<QDiscordChannel> channel,
                                        bool tts)
{
    if (!_loggedIn)
        return;

    if (!channel)
        return;

    quint64 id = channel->id();
    QJsonObject object;
    object["content"] = content;

    if(tts)
        object["tts"] = true;

    doRequest(object, QDiscordRoutes::Messages::sendMessage(id),
    [this, channel](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit messageSendFailed(reply->error());
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QDiscordMessage message(document.object(), channel);
        emit messageSent(message);
    });
}

void QDiscordRestComponent::sendMessage(const QString &content,
                                        const quint64 &channelId,
                                        bool tts)
{
    if (!_loggedIn)
        return;

    QJsonObject object;
    object["content"] = content;

    if (tts)
        object["tts"] = true;

    doRequest(object, QDiscordRoutes::Messages::sendMessage(channelId),
    [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit messageSendFailed(reply->error());
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QDiscordMessage message(document.object(), QSharedPointer<QDiscordChannel>());
        emit messageSent(message);
    });
}

void QDiscordRestComponent::deleteMessage(const QDiscordMessage &message)
{
    deleteMessage(message.channelId(), message.id());
}

void QDiscordRestComponent::deleteMessage(const quint64 &channelId,
                                          const quint64 &messageId)
{
    if (!_loggedIn)
        return;

    doRequest(QDiscordRoutes::Messages::deleteMessage(channelId, messageId),
    [this, messageId](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit messageDeleteFailed(reply->error());
            return;
        }

        emit messageDeleted(messageId);
    });
}

void QDiscordRestComponent::editMessage(const QString &newContent,
                                        const QDiscordMessage &message)
{
    editMessage(newContent, message.channelId(), message.id());
}

void QDiscordRestComponent::editMessage(const QString &newContent,
                                        const quint64 &channelId,
                                        const quint64 &messageId)
{
    if (!_loggedIn)
        return;

    QJsonObject object;
    object["content"] = newContent;

    doRequest(object, QDiscordRoutes::Messages::editMessage(channelId, messageId),
    [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit messageEditFailed(reply->error());
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QDiscordMessage newMessage(document.object(), QSharedPointer<QDiscordChannel>());
        emit messageEdited(newMessage);
    });
}

void QDiscordRestComponent::logout()
{
    if (_authentication.isEmpty())
        return;

    if (!_loggedIn)
        return;

    _self.reset();

    QJsonObject object;
    object["token"] = _authentication;
    _authentication = "";
    _loggedIn = false;

    doRequest(object, QDiscordRoutes::Self::logout(),
    [this](QNetworkReply*)
    {
        emit loggedOut();
    });
}

void QDiscordRestComponent::getEndpoint()
{
    if(!_loggedIn)
        return;

    doRequest(QDiscordRoutes::Self::gateway(),
    [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit endpointAcquireFailed(reply->error());
            return;
        }

        emit endpointAcquired(
            QJsonDocument::fromJson(
                reply->readAll()
            ).object().value("url").toString()
        );
    });
}

void QDiscordRestComponent::setChannelName(const QString &name,
                                           QSharedPointer<QDiscordChannel> channel)
{
    if (!_loggedIn)
        return;

    if (!channel)
        return;

    setChannelName(name, channel->id());
}

void QDiscordRestComponent::setChannelName(const QString &name,
                                           const quint64 &channelId)
{
    if (!_loggedIn)
        return;

    QJsonObject object;
    object["name"] = name;

    doRequest(object, QDiscordRoutes::Channels::modifyChannel(channelId),
    [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit channelUpdateFailed(reply->error());
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QDiscordChannel updatedChannel(document.object());
        emit channelUpdated(updatedChannel);
    });
}

void QDiscordRestComponent::setChannelPosition(int position,
                                               QSharedPointer<QDiscordChannel> channel)
{
    if (!_loggedIn)
        return;

    if (!channel)
        return;

    setChannelPosition(position, channel->id());
}

void QDiscordRestComponent::setChannelPosition(int position,
                                               const quint64& channelId)
{
    if (!_loggedIn)
        return;

    QJsonObject object;
    object["position"] = position;

    doRequest(object, QDiscordRoutes::Channels::modifyChannel(channelId),
    [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit channelUpdateFailed(reply->error());
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QDiscordChannel updatedChannel(document.object());
        emit channelUpdated(updatedChannel);
    });
}

void QDiscordRestComponent::setChannelTopic(const QString &topic,
                                            QSharedPointer<QDiscordChannel> channel)
{
    if (!_loggedIn)
        return;

    if (!channel)
        return;

    if (channel->type() != QDiscordChannel::ChannelType::Text)
        return;

    setChannelTopic(topic, channel->id());
}

void QDiscordRestComponent::setChannelTopic(const QString &topic,
                                            const quint64 &channelId)
{
    if (!_loggedIn)
        return;

    QJsonObject object;
    object["topic"] = topic;

    doRequest(object, QDiscordRoutes::Channels::modifyChannel(channelId),
    [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit channelUpdateFailed(reply->error());
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QDiscordChannel updatedChannel(document.object());
        emit channelUpdated(updatedChannel);
    });
}

void QDiscordRestComponent::setChannelBitrate(int bitrate,
                                              QSharedPointer<QDiscordChannel> channel)
{
    if (!_loggedIn)
        return;

    if (!channel)
        return;

    if (channel->type() != QDiscordChannel::ChannelType::Voice)
        return;

    setChannelBitrate(bitrate, channel->id());
}

void QDiscordRestComponent::setChannelBitrate(int bitrate,
                                              const quint64 &channelId)
{
    if (!_loggedIn)
        return;

    QJsonObject object;
    object["bitrate"] = bitrate;

    doRequest(object, QDiscordRoutes::Channels::modifyChannel(channelId),
    [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit channelUpdateFailed(reply->error());
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QDiscordChannel updatedChannel(document.object());
        emit channelUpdated(updatedChannel);
    });
}

void QDiscordRestComponent::setChannelUserLimit(int limit, QSharedPointer<QDiscordChannel> channel)
{
    if (!_loggedIn)
        return;

    if (!channel)
        return;

    if (channel->type() != QDiscordChannel::ChannelType::Voice)
        return;

    setChannelUserLimit(limit, channel->id());
}

void QDiscordRestComponent::setChannelUserLimit(int limit,
                                                const quint64 &channelId)
{
    if (!_loggedIn)
        return;

    QJsonObject object;
    object["user_limit"] = limit;

    doRequest(object, QDiscordRoutes::Channels::modifyChannel(channelId),
    [this](QNetworkReply* reply)
    {
        if (reply->error() != QNetworkReply::NoError)
        {
            emit channelUpdateFailed(reply->error());
            return;
        }

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QDiscordChannel updatedChannel(document.object());
        emit channelUpdated(updatedChannel);
    });
}

void QDiscordRestComponent::setSelf(QSharedPointer<QDiscordUser> self)
{
    _self = self;
}

template<class Functor>
void QDiscordRestComponent::doRequest(const QDiscordRoute &url,
                                      Functor function)
{
    QNetworkRequest request(QUrl(url.fullUrl()));
    if (!_authentication.isEmpty())
        request.setRawHeader("Authorization", _authentication.toUtf8());

    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QDiscordUtilities::userAgent());

    QNetworkReply *reply = nullptr;
    switch(url.method())
    {
    case QDiscordRoute::Method::DELETE:
        reply = _manager.deleteResource(request);
        break;

    case QDiscordRoute::Method::GET:
        reply = _manager.get(request);
        break;

    case QDiscordRoute::Method::PATCH:
    {
        QBuffer* buffer = new QBuffer();
        buffer->open(QBuffer::ReadWrite);
        request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
        reply = _manager.sendCustomRequest(request, "PATCH", buffer);
        connect(reply, &QNetworkReply::finished, buffer, &QBuffer::deleteLater);
    }
        break;

    case QDiscordRoute::Method::POST:
        request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
        reply = _manager.post(request, "");
        break;

    case QDiscordRoute::Method::PUT:
        request.setHeader(QNetworkRequest::ContentLengthHeader, 0);
        reply = _manager.put(request, "");
        break;

    default:
        return;
        break;
    }

    connect(reply, &QNetworkReply::finished, this, [this, function]() {
        QNetworkReply *rply = static_cast<QNetworkReply*>(sender());

        if (!rply)
            return;

        function(rply);

        rply->deleteLater();
    });

#ifdef QDISCORD_LIBRARY_DEBUG
    qDebug() << this << "did" << url;
#endif
}

template<class Functor>
void QDiscordRestComponent::doRequest(const QJsonObject &object,
                                      const QDiscordRoute &url,
                                      Functor function)
{
    QNetworkRequest request(QUrl(url.fullUrl()));
    if (!_authentication.isEmpty())
        request.setRawHeader("Authorization", _authentication.toUtf8());

    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QDiscordUtilities::userAgent());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = nullptr;
    QJsonDocument document;
    switch(url.method())
    {
    case QDiscordRoute::Method::DELETE:
        reply = _manager.deleteResource(request);
        break;

    case QDiscordRoute::Method::GET:
        reply = _manager.get(request);
        break;

    case QDiscordRoute::Method::PATCH:
    {
        QBuffer* buffer = new QBuffer();
        buffer->open(QBuffer::ReadWrite);

        document.setObject(object);
        buffer->write(document.toJson(QJsonDocument::Compact));
        buffer->seek(0);
        reply = _manager.sendCustomRequest(request, "PATCH", buffer);
        connect(reply, &QNetworkReply::finished, buffer, &QBuffer::deleteLater);
    }
        break;

    case QDiscordRoute::Method::POST:
        document.setObject(object);
        reply = _manager.post(request, document.toJson(QJsonDocument::Compact));
        break;

    case QDiscordRoute::Method::PUT:
        document.setObject(object);
        reply = _manager.put(request, document.toJson(QJsonDocument::Compact));
        break;

    default:
        return;
    }

    connect(reply, &QNetworkReply::finished, this, [this, function]() {
        QNetworkReply* rply = static_cast<QNetworkReply*>(sender());

        if (!rply)
            return;

        function(rply);

        rply->deleteLater();
    });

#ifdef QDISCORD_LIBRARY_DEBUG
    qDebug() << this << "did" << url;
#endif
}

template<class Functor>
void QDiscordRestComponent::doRequest(const QJsonArray &array,
                                      const QDiscordRoute &url,
                                      Functor function)
{
    QNetworkRequest request(QUrl(url.fullUrl()));
    if (!_authentication.isEmpty())
        request.setRawHeader("Authorization", _authentication.toUtf8());

    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QDiscordUtilities::userAgent());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply;
    QJsonDocument document;
    switch(url.method())
    {
    case QDiscordRoute::Method::DELETE:
        reply = _manager.deleteResource(request);
        break;

    case QDiscordRoute::Method::GET:
        reply = _manager.get(request);
        break;

    case QDiscordRoute::Method::PATCH:
    {
        QBuffer* buffer = new QBuffer();
        buffer->open(QBuffer::ReadWrite);
        document.setArray(array);
        buffer->write(document.toJson(QJsonDocument::Compact));
        buffer->seek(0);
        reply = _manager.sendCustomRequest(request, "PATCH", buffer);
        connect(reply, &QNetworkReply::finished, buffer, &QBuffer::deleteLater);
    }
        break;

    case QDiscordRoute::Method::POST:
        document.setArray(array);
        reply = _manager.post(request, document.toJson(QJsonDocument::Compact));
        break;

    case QDiscordRoute::Method::PUT:
        document.setArray(array);
        reply = _manager.put(request, document.toJson(QJsonDocument::Compact));
        break;

    default:
        return;
    }
    connect(reply, &QNetworkReply::finished, this, [this, function]() {
        QNetworkReply* rply = static_cast<QNetworkReply*>(sender());

        if (!rply)
            return;

        function(rply);

        rply->deleteLater();
    });

#ifdef QDISCORD_LIBRARY_DEBUG
    qDebug() << this << "did" << url;
#endif
}

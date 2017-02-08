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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.	 If not, see <http://www.gnu.org/licenses/>.
 */

#include "qdiscordroute.hpp"

QDiscordRoute::QDiscordRoute(Method method, QString url)
{
	route(method, url);
}

QDiscordRoute::Method QDiscordRoute::method() const
{
	return _method;
}

QString QDiscordRoute::bucketUrl() const
{
	return _bucketUrl;
}

QString QDiscordRoute::fullUrl() const
{
	return _fullUrl;
}

QString QDiscordRoute::routeString(QString url)
{
	return url;
}

void QDiscordRoute::route(QDiscordRoute::Method method, QString url)
{
	_method = method;
	_fullUrl = url;
	_bucketUrl = url;
}

// Self
QDiscordRoute QDiscordRoutes::Self::getSelf()
{
	return QDiscordRoute(QDiscordRoute::Method::GET,
						 QDiscordUtilities::endPoints.me);
}

QDiscordRoute QDiscordRoutes::Self::modifySelf()
{
	return QDiscordRoute(QDiscordRoute::Method::PATCH,
						 QDiscordUtilities::endPoints.me);
}

QDiscordRoute QDiscordRoutes::Self::getGuilds()
{
	return QDiscordRoute(QDiscordRoute::Method::GET,
						 QDiscordUtilities::endPoints.me + "/guilds");
}

QDiscordRoute QDiscordRoutes::Self::leaveGuild(const quint64& guild)
{
	return QDiscordRoute(QDiscordRoute::Method::DELETE,
						 QDiscordUtilities::endPoints.me + "/guilds/{guildm}",
                                                 QString::number(guild));
}

QDiscordRoute QDiscordRoutes::Self::getPrivateChannels()
{
	return QDiscordRoute(QDiscordRoute::Method::GET,
						 QDiscordUtilities::endPoints.me + "/channels");
}

QDiscordRoute QDiscordRoutes::Self::createPrivateChannel()
{
	return QDiscordRoute(QDiscordRoute::Method::POST,
						 QDiscordUtilities::endPoints.me + "/channels");
}

QDiscordRoute QDiscordRoutes::Self::gateway()
{
	return QDiscordRoute(QDiscordRoute::Method::GET,
						 QDiscordUtilities::endPoints.gateway);
}

QDiscordRoute QDiscordRoutes::Self::userSettings()
{
	return QDiscordRoute(QDiscordRoute::Method::GET,
						 QDiscordUtilities::endPoints.me + "/settings");
}

QDiscordRoute QDiscordRoutes::Self::login()
{
	return QDiscordRoute(QDiscordRoute::Method::POST,
						 QDiscordUtilities::endPoints.login);
}

QDiscordRoute QDiscordRoutes::Self::logout()
{
	return QDiscordRoute(QDiscordRoute::Method::POST,
						 QDiscordUtilities::endPoints.logout);
}

// Users
QDiscordRoute QDiscordRoutes::Users::getUser(const quint64& user)
{
	return QDiscordRoute(QDiscordRoute::Method::GET,
						 QDiscordUtilities::endPoints.users + "/{user}",
                                                 QString::number(user));
}

// Guilds
QDiscordRoute QDiscordRoutes::Guilds::getGuild(const quint64& guild)
{
	return QDiscordRoute(QDiscordRoute::Method::GET,
						 QDiscordUtilities::endPoints.guilds + "/{guild}",
                                                 QString::number(guild));
}

QDiscordRoute QDiscordRoutes::Guilds::modifyGuild(const quint64& guild)
{
	return QDiscordRoute(QDiscordRoute::Method::PATCH,
						 QDiscordUtilities::endPoints.guilds + "/{guild}",
                                                 QString::number(guild));
}

QDiscordRoute QDiscordRoutes::Guilds::createChannel(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.guilds + "/{guild}/channels",
                                QString::number(guild)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::getChannels(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.guilds + "/{guild}/channels",
                                QString::number(guild)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::modifyChannels(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PATCH,
				QDiscordUtilities::endPoints.guilds + "/{guild}/channels",
                                QString::number(guild)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::getBans(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.guilds + "/{guild}/bans",
                                QString::number(guild)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::ban(const quint64& guild,
                                          const quint64& user)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PUT,
				QDiscordUtilities::endPoints.guilds + "/{guild}/bans/{user}",
                                QString::number(guild),
                                QString::number(user)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::banWithDelete(const quint64& guild,
                                                    const quint64& user,
													int days)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PUT,
				QDiscordUtilities::endPoints.guilds +
					"/{guild}/bans/{user}?delete-message-days={days}",
                                QString::number(guild),
                                QString::number(user),
				QString::number(days)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::unban(const quint64& guild,
                                            const quint64& user)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.guilds + "/{guild}/bans/{user}",
                                QString::number(guild),
                                QString::number(user)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::kickMember(const quint64& guild,
                                                 const quint64& user)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.guilds + "/{guild}/members/{user}",
                                QString::number(guild),
                                QString::number(user)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::modifyMember(const quint64& guild,
                                                   const quint64& user)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PATCH,
				QDiscordUtilities::endPoints.guilds + "/{guild}/members/{user}",
                                QString::number(guild),
                                QString::number(user)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::modifySelfNick(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PATCH,
				QDiscordUtilities::endPoints.guilds + "/{guild}/@me/nick",
                                QString::number(guild)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::prunableCount(const quint64& guild,
													int days)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.guilds +
					"/{guild}/prune?days={days}",
                                QString::number(guild),
				QString::number(days)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::pruneMembers(const quint64& guild,
												   int days)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.guilds +
					"/{guild}/prune?days={days}",
                                QString::number(guild),
				QString::number(days)
				);
}

QDiscordRoute QDiscordRoutes::Guilds::createGuild()
{
	return QDiscordRoute(QDiscordRoute::Method::POST,
						 QDiscordUtilities::endPoints.guilds);
}

QDiscordRoute QDiscordRoutes::Guilds::deleteGuild(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.guilds + "/{guildm}/delete",
                                QString::number(guild)
				);
}

// Emotes
QDiscordRoute QDiscordRoutes::Emotes::modifyEmote(const quint64& guild,
                                                                                                  const quint64& emote)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PATCH,
				QDiscordUtilities::endPoints.guilds +
					"/{guildm}/emojis/{emote}",
                                QString::number(guild),
                                QString::number(emote)
				);
}

QDiscordRoute QDiscordRoutes::Emotes::deleteEmote(const quint64& guild,
                                                                                                  const quint64& emote)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.guilds +
					"/{guildm}/emojis/{emote}",
                                QString::number(guild),
                                QString::number(emote)
				);
}

QDiscordRoute QDiscordRoutes::Emotes::createEmote(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.guilds + "/{guildm}/emojis",
                                QString::number(guild)
				);
}

// Roles
QDiscordRoute QDiscordRoutes::Roles::getRoles(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.guilds + "/{guild}/roles",
                                QString::number(guild)
				);
}

QDiscordRoute QDiscordRoutes::Roles::createRole(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.guilds + "/{guild}/roles",
                                QString::number(guild)
				);
}

QDiscordRoute QDiscordRoutes::Roles::getRole(const quint64& guild,
                                             const quint64& role)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.guilds + "/{guild}/roles/{role}",
                                QString::number(guild),
                                QString::number(role)
				);
}

QDiscordRoute QDiscordRoutes::Roles::modifyRole(const quint64& guild,
                                                const quint64& role)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PATCH,
				QDiscordUtilities::endPoints.guilds + "/{guild}/roles/{role}",
                                QString::number(guild),
                                QString::number(role)
				);
}

QDiscordRoute QDiscordRoutes::Roles::deleteRole(const quint64& guild,
                                                const quint64& role)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.guilds + "/{guild}/roles/{role}",
                                QString::number(guild),
                                QString::number(role)
				);
}

// Channels
QDiscordRoute QDiscordRoutes::Channels::deleteChannel(const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.channels + "/{channelm}",
                                QString::number(channel)
				);
}

QDiscordRoute QDiscordRoutes::Channels::modifyChannel(const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PATCH,
				QDiscordUtilities::endPoints.channels + "/{channel}",
                                QString::number(channel)
				);
}

QDiscordRoute QDiscordRoutes::Channels::sendTyping(const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.channels + "/{channel}/typing",
                                QString::number(channel)
				);
}

QDiscordRoute QDiscordRoutes::Channels::getPermissions(const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/permissions",
                                QString::number(channel)
				);
}

QDiscordRoute QDiscordRoutes::Channels::getPermissionsOverride(
		const quint64& channel,
		const QString& permissionsOverride)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/permissions/{permoverride}",
                                QString::number(channel),
				permissionsOverride
				);
}

QDiscordRoute QDiscordRoutes::Channels::createPermissionsOverride(
		const quint64& channel,
		const QString& permissionsOverride)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PUT,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/permissions/{permoverride}",
                                QString::number(channel),
				permissionsOverride
				);
}

QDiscordRoute QDiscordRoutes::Channels::modifyPermissionsOverride(
		const quint64& channel,
		const QString& permissionsOverride)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PUT,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/permissions/{permoverride}",
                                QString::number(channel),
				permissionsOverride
				);
}

QDiscordRoute QDiscordRoutes::Channels::deletePermissionsOverride(
		const quint64& channel,
		const QString& permissionsOverride)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/permissions/{permoverride}",
                                QString::number(channel),
				permissionsOverride
	);
}

// Messages
QDiscordRoute QDiscordRoutes::Messages::sendMessage(const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.channels + "/{channel}/messages",
                                QString::number(channel)
				);
}

QDiscordRoute QDiscordRoutes::Messages::editMessage(const quint64& channel,
													const quint64& message)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PATCH,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages/{message}",
                                QString::number(channel),
                                QString::number(message)
				);
}

QDiscordRoute QDiscordRoutes::Messages::deleteMessage(const quint64& channel,
													  const quint64& message)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages/{message}",
                                QString::number(channel),
                                QString::number(message)
				);
}

QDiscordRoute QDiscordRoutes::Messages::getPinnedMessages(
		const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/pins",
                                QString::number(channel)
				);
}

QDiscordRoute QDiscordRoutes::Messages::addPinnedMessage(
			const quint64& channel,
			const quint64& message)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PUT,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/pins/{message}",
                                QString::number(channel),
                                QString::number(message)
				);
}

QDiscordRoute QDiscordRoutes::Messages::removePinnedMessage(
			const quint64& channel,
			const quint64& message)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/pins/{message}",
                                QString::number(channel),
                                QString::number(message)
				);
}

QDiscordRoute QDiscordRoutes::Messages::addReaction(const quint64& channel,
													const quint64& message,
													const QString& reactionCode)
{
	return QDiscordRoute(
				QDiscordRoute::Method::PUT,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages/{message}/reactions/{reacion}/@me",
                                QString::number(channel),
                                QString::number(message),
				reactionCode
				);
}

QDiscordRoute QDiscordRoutes::Messages::removeReaction(
			const quint64& channel,
			const quint64& message,
			const QString& reactionCode,
			const quint64& user)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages/{message}/reactions/{reaction}/{user}",
                                QString::number(channel),
                                QString::number(message),
				reactionCode,
                                QString::number(user)
				);
}

QDiscordRoute QDiscordRoutes::Messages::removeAllReactions(
			const quint64& channel,
			const quint64& message)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages/{message}/reactions",
                                QString::number(channel),
                                QString::number(message)
				);
}

QDiscordRoute QDiscordRoutes::Messages::getReactionUsers(
			const quint64& channel,
			const quint64& message,
			const QString& reactionCode,
			int limit)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages/{message}"
					"/reactions/{reaction}?limit={limit}",
                                QString::number(channel),
                                QString::number(message),
				reactionCode,
				QString::number(limit)
				);
}

QDiscordRoute QDiscordRoutes::Messages::getMessageHistory(
			const quint64& channel,
			int limit)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages?limit={limit}",
                                QString::number(channel),
                                QString::number(limit)
				);
}

QDiscordRoute QDiscordRoutes::Messages::getMessageHistoryBefore(
			const quint64& channel,
			int limit,
			const QString& before)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages?limit={limit}&before={before}",
                                QString::number(channel),
				QString::number(limit),
				before
				);
}

QDiscordRoute QDiscordRoutes::Messages::getMessageHistoryAfter(
			const quint64& channel,
			int limit,
			const QString& after)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages?limit={limit}&after={after}",
                                QString::number(channel),
				QString::number(limit),
				after
				);
}

QDiscordRoute QDiscordRoutes::Messages::getMessageHistoryAround(
			const quint64& channel,
			int limit,
			const QString& around)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages?limit={limit}&around={around}",
                                QString::number(channel),
				QString::number(limit),
				around
				);
}

QDiscordRoute QDiscordRoutes::Messages::getMessage(
			const quint64& channel,
			const quint64& message)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages/{message}",
                                QString::number(channel),
                                QString::number(message)
				);
}

QDiscordRoute QDiscordRoutes::Messages::deleteMessages(const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.channels +
					"/{channel}/messages/bulk-delete",
                                QString::number(channel)
				);
}

// Invites
QDiscordRoute QDiscordRoutes::Invites::getInvite(const QString& code)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.apiBase + "/invites/{code}",
				code
				);
}

QDiscordRoute QDiscordRoutes::Invites::deleteInvite(const QString& code)
{
	return QDiscordRoute(
				QDiscordRoute::Method::DELETE,
				QDiscordUtilities::endPoints.apiBase + "/invites/{code}",
				code
				);
}

QDiscordRoute QDiscordRoutes::Invites::getGuildInvites(const quint64& guild)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.guilds + "/{guild}/invites",
                                QString::number(guild)
				);
}

QDiscordRoute QDiscordRoutes::Invites::getChannelInvites(const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::GET,
				QDiscordUtilities::endPoints.channels + "/{channel}/invites",
                                QString::number(channel)
				);
}

QDiscordRoute QDiscordRoutes::Invites::createInvite(const quint64& channel)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.channels + "/{channel}/invites",
                                QString::number(channel)
				);
}

QDiscordRoute QDiscordRoutes::Invites::acceptInvite(const QString& code)
{
	return QDiscordRoute(
				QDiscordRoute::Method::POST,
				QDiscordUtilities::endPoints.apiBase + "/invites/{code}",
				code
				);
}

#ifdef QDISCORD_LIBRARY_DEBUG
QDebug& operator<<(QDebug& d, const QDiscordRoute& route)
{
	d.nospace() << "QDiscordRoute(Method: ";
	switch(route.method())
	{
	case QDiscordRoute::Method::DELETE:
		d << "DELETE";
		break;
	case QDiscordRoute::Method::GET:
		d << "GET";
		break;
	case QDiscordRoute::Method::PATCH:
		d << "PATCH";
		break;
	case QDiscordRoute::Method::POST:
		d << "POST";
		break;
	case QDiscordRoute::Method::PUT:
		d << "PUT";
		break;
	default:
		d << "?";
	}

	d << " Full URL: " << route.fullUrl();
	d << " Bucket URL: " << route.bucketUrl();
	d << ")";

	return d.maybeSpace();
}
#endif

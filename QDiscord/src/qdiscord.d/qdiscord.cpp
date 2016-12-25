/*
 * QDiscord - An unofficial C++ and Qt wrapper for the Discord API.
 * Copyright (C) 2016 george99g
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

#include "qdiscord.hpp"

QDiscord::QDiscord(QObject* parent) : QObject(parent)
{
	connectComponents();
	_signalsConnected = false;

#ifdef QDISCORD_LIBRARY_DEBUG
	qDebug()<<this<<"constructed";
#endif
}

void QDiscord::login(const QString& email, const QString& password)
{
	connectDiscordSignals();
	_signalsConnected = true;
	_rest.login(email, password);
}

void QDiscord::login(const QString& token)
{
	connectDiscordSignals();
	_signalsConnected = true;
	_rest.login(token);
}

void QDiscord::logout()
{
	_ws.close();
	_rest.logout();
}

void QDiscord::tokenVerfified(const QString& token)
{
	_token = token;
	_rest.getEndpoint();
}

void QDiscord::endpointAcquired(const QString& endpoint)
{
	_ws.connectToEndpoint(endpoint, _token);
}

void QDiscord::connectComponents()
{
	connect(&_ws, &QDiscordWsComponent::readyReceived,
			&_state, &QDiscordStateComponent::readyReceived);
	connect(&_ws, &QDiscordWsComponent::guildCreateReceived,
			&_state, &QDiscordStateComponent::guildCreateReceived);
	connect(&_ws, &QDiscordWsComponent::guildDeleteReceived,
			&_state, &QDiscordStateComponent::guildDeleteReceived);
	connect(&_ws, &QDiscordWsComponent::guildBanAddReceived,
			&_state, &QDiscordStateComponent::guildBanAddReceived);
	connect(&_ws, &QDiscordWsComponent::guildBanRemoveReceived,
			&_state, &QDiscordStateComponent::guildBanRemoveReceived);
	connect(&_ws, &QDiscordWsComponent::guildIntegrationsUpdateRecevied,
			&_state, &QDiscordStateComponent::guildIntegrationsUpdateRecevied);
	connect(&_ws, &QDiscordWsComponent::guildMemberAddReceived,
			&_state, &QDiscordStateComponent::guildMemberAddReceived);
	connect(&_ws, &QDiscordWsComponent::guildMemberRemoveReceived,
			&_state, &QDiscordStateComponent::guildMemberRemoveReceived);
	connect(&_ws, &QDiscordWsComponent::guildMemberUpdateReceived,
			&_state, &QDiscordStateComponent::guildMemberUpdateReceived);
	connect(&_ws, &QDiscordWsComponent::guildRoleCreateReceived,
			&_state, &QDiscordStateComponent::guildRoleCreateReceived);
	connect(&_ws, &QDiscordWsComponent::guildRoleDeleteReceived,
			&_state, &QDiscordStateComponent::guildRoleDeleteReceived);
	connect(&_ws, &QDiscordWsComponent::guildRoleUpdateReceived,
			&_state, &QDiscordStateComponent::guildRoleUpdateReceived);
	connect(&_ws, &QDiscordWsComponent::guildUpdateReceived,
			&_state, &QDiscordStateComponent::guildUpdateReceived);
	connect(&_ws, &QDiscordWsComponent::messageCreateReceived,
			&_state, &QDiscordStateComponent::messageCreateReceived);
	connect(&_ws, &QDiscordWsComponent::messageDeleteReceived,
			&_state, &QDiscordStateComponent::messageDeleteReceived);
	connect(&_ws, &QDiscordWsComponent::messageUpdateReceived,
			&_state, &QDiscordStateComponent::messageUpdateReceived);
	connect(&_ws, &QDiscordWsComponent::presenceUpdateReceived,
			&_state, &QDiscordStateComponent::presenceUpdateReceived);
	connect(&_ws, &QDiscordWsComponent::typingStartReceived,
			&_state, &QDiscordStateComponent::typingStartReceived);
	connect(&_ws, &QDiscordWsComponent::userSettingsUpdateReceived,
			&_state, &QDiscordStateComponent::userSettingsUpdateReceived);
	connect(&_ws, &QDiscordWsComponent::voiceStateUpdateReceived,
			&_state, &QDiscordStateComponent::voiceStateUpdateReceived);
	connect(&_ws, &QDiscordWsComponent::channelCreateReceived,
			&_state, &QDiscordStateComponent::channelCreateReceived);
	connect(&_ws, &QDiscordWsComponent::channelDeleteReceived,
			&_state, &QDiscordStateComponent::channelDeleteReceived);
	connect(&_ws, &QDiscordWsComponent::channelUpdateReceived,
			&_state, &QDiscordStateComponent::channelUpdateReceived);
	connect(&_ws, &QDiscordWsComponent::disconnected,
			&_state, &QDiscordStateComponent::clear);
	connect(&_ws, &QDiscordWsComponent::error,
			&_state, &QDiscordStateComponent::clear);
	connect(&_state, &QDiscordStateComponent::selfCreated,
			&_rest, &QDiscordRestComponent::selfCreated);

}

void QDiscord::connectDiscordSignals()
{
	connect(&_rest, &QDiscordRestComponent::tokenVerified,
			this, &QDiscord::tokenVerfified);
	connect(&_rest, &QDiscordRestComponent::endpointAcquired,
			this, &QDiscord::endpointAcquired);
	connect(&_rest, &QDiscordRestComponent::loginFailed,
			this, &QDiscord::loginFailed);
	connect(&_rest, &QDiscordRestComponent::endpointAcquireFailed,
			this, &QDiscord::loginFailed);
	connect(&_ws, &QDiscordWsComponent::loginFailed,
			this, &QDiscord::loginFailed);
	connect(&_ws, &QDiscordWsComponent::loginSuccess,
			this, &QDiscord::loginSuccess);
	connect(&_ws, &QDiscordWsComponent::disconnected,
			this, &QDiscord::disconnected);
	connect(&_ws, &QDiscordWsComponent::error,
			this, &QDiscord::disconnected);
	connect(&_rest, &QDiscordRestComponent::loggedOut,
			this, &QDiscord::logoutFinished);
}

void QDiscord::disconnectDiscordSignals()
{
	disconnect(&_rest, &QDiscordRestComponent::tokenVerified,
			   this, &QDiscord::tokenVerfified);
	disconnect(&_rest, &QDiscordRestComponent::endpointAcquired,
			   this, &QDiscord::endpointAcquired);
	disconnect(&_rest, &QDiscordRestComponent::loginFailed,
			   this, &QDiscord::loginFailed);
	disconnect(&_rest, &QDiscordRestComponent::endpointAcquireFailed,
			   this, &QDiscord::loginFailed);
	disconnect(&_ws, &QDiscordWsComponent::loginFailed,
			   this, &QDiscord::loginFailedReceived);
	disconnect(&_ws, &QDiscordWsComponent::loginSuccess,
			   this, &QDiscord::loginSuccessRecevied);
	disconnect(&_ws, &QDiscordWsComponent::disconnected,
			   this, &QDiscord::disconnected);
	disconnect(&_ws, &QDiscordWsComponent::error,
			   this, &QDiscord::disconnected);
	disconnect(&_rest, &QDiscordRestComponent::loggedOut,
			   this, &QDiscord::logoutFinished);
}

void QDiscord::logoutFinished()
{
	if(_signalsConnected)
	{
		_signalsConnected = false;
		disconnectDiscordSignals();
	}
	emit loggedOut();
}

void QDiscord::loginSuccessRecevied()
{
	emit loginSuccess();
}

void QDiscord::loginFailedReceived()
{
	emit loginFailed();
}

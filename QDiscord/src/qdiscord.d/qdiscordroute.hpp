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

#ifndef QDISCORDROUTE_HPP
#define QDISCORDROUTE_HPP

/*
 * This code is based off of the following code by FiniteReality:
 * https://github.com/FiniteReality/disccord/blob/master/include/rest/route.hpp
 * https://github.com/FiniteReality/disccord/blob/master/lib/rest/route.cpp
 * And the following code by DV8FromTheWorld:
 * https://github.com/DV8FromTheWorld/JDA/blob/master/src/main/java/net/dv8tion/jda/core/requests/Route.java
 */

#include <QString>
#include "qdiscordutilities.hpp"

class QDiscordRoute
{
public:
	enum class Method {
		GET, POST, PUT,
		DELETE, PATCH
	};
	QDiscordRoute(Method method, QString url);
	template<class TFirst, class... TArgs>
	QDiscordRoute(Method method, QString base, TFirst first, TArgs... args)
	{
		route(method, base, first, args...);
	}
	Method method() const;
	QString bucketUrl() const;
	QString fullUrl() const;

private:
	template<class TFirst, class... TArgs>
	static QString routeString(QString base, TFirst first, TArgs... args)
	{
		int start = base.indexOf('{');
		int end = base.indexOf('}', start);

		return routeString(base.replace(start, end, first), args...);
	}
	static QString routeString(QString url);
	template<class TFirst, class... TArgs>
	void route(Method method, QString base, TFirst first, TArgs... args)
	{
		int start = base.indexOf('{');
		int end = base.indexOf('}', start) - start + 1;

		QString parameter = base.mid(start, end);
		if(parameter == "{channel}" || parameter == "{guild}")
			route(method, base.replace(start, end, first), args...);
		else
		{
			_method = method;
			_bucketUrl = base;
			_fullUrl = routeString(base.replace(start, end, first), args...);
		}
	}
	void route(Method method, QString url);
	Method _method;
	QString _bucketUrl;
	QString _fullUrl;
};

#ifdef QDISCORD_LIBRARY_DEBUG
QDebug& operator<<(QDebug& d, const QDiscordRoute& route);
#endif

namespace QDiscordRoutes {
	namespace Self {
		QDiscordRoute getSelf();
		QDiscordRoute modifySelf();
		QDiscordRoute getGuilds();
		QDiscordRoute leaveGuild(const quint64& guild);
		QDiscordRoute getPrivateChannels();
		QDiscordRoute createPrivateChannel();
		QDiscordRoute gateway();

		//Client-only
		QDiscordRoute userSettings();
		QDiscordRoute login();
		QDiscordRoute logout();
	}

	namespace Users {
		QDiscordRoute getUser(const quint64& user);
	}

	namespace Guilds {
		QDiscordRoute getGuild(const quint64& guild);
		QDiscordRoute modifyGuild(const quint64& guild);
		QDiscordRoute createChannel(const quint64& guild);
		QDiscordRoute getChannels(const quint64& guild);
		QDiscordRoute modifyChannels(const quint64& guild);
		QDiscordRoute getBans(const quint64& guild);
		QDiscordRoute ban(const quint64& guild, const quint64& user);
		QDiscordRoute banWithDelete(
				const quint64& guild,
				const quint64& user,
				int days
		);
		QDiscordRoute unban(const quint64& guild, const quint64& user);
		QDiscordRoute kickMember(const quint64& guild, const quint64& user);
		QDiscordRoute modifyMember(const quint64& guild, const quint64& user);
		QDiscordRoute modifySelfNick(const quint64& guild);
		QDiscordRoute prunableCount(const quint64& guild, int days);
		QDiscordRoute pruneMembers(const quint64& guild, int days);

		//Client-only
		QDiscordRoute createGuild();
		QDiscordRoute deleteGuild(const quint64& guild);
	}

	namespace Emotes {
		QDiscordRoute modifyEmote(const quint64& guild, const quint64& emote);
		QDiscordRoute deleteEmote(const quint64& guild, const quint64& emote);
		QDiscordRoute createEmote(const quint64& guild);
	}

	namespace Roles {
		QDiscordRoute getRoles(const quint64& guild);
		QDiscordRoute createRole(const quint64& guild);
		QDiscordRoute getRole(const quint64& guild, const quint64& role);
		QDiscordRoute modifyRole(const quint64& guild, const quint64& role);
		QDiscordRoute deleteRole(const quint64& guild, const quint64& role);
	}

	namespace Channels {
		QDiscordRoute deleteChannel(const quint64& channel);
		QDiscordRoute modifyChannel(const quint64& channel);
		QDiscordRoute sendTyping(const quint64& channel);
		QDiscordRoute getPermissions(const quint64& channel);
		QDiscordRoute getPermissionsOverride(
				const quint64& channel,
				const QString& permissionsOverride
		);
		QDiscordRoute createPermissionsOverride(
				const quint64& channel,
				const QString& permissionsOverride
		);
		QDiscordRoute modifyPermissionsOverride(
				const quint64& channel,
				const QString& permissionsOverride
		);
		QDiscordRoute deletePermissionsOverride(
				const quint64& channel,
				const QString& permissionsOverride
		);
	}

	namespace Messages {
		QDiscordRoute sendMessage(const quint64& channel);
		QDiscordRoute editMessage(const quint64& channel,
								  const quint64& message);
		QDiscordRoute deleteMessage(const quint64& channel,
									const quint64& message);
                QDiscordRoute bulkDeleteMessages(const quint64 &channel);
		QDiscordRoute getPinnedMessages(const quint64& channel);
		QDiscordRoute addPinnedMessage(const quint64& channel,
									   const quint64& message);
		QDiscordRoute removePinnedMessage(const quint64& channel,
										  const quint64& message);

		QDiscordRoute addReaction(const quint64& channel,
								  const quint64& message,
								  const QString& reactionCode);
		QDiscordRoute removeReaction(const quint64& channel,
									 const quint64& message,
									 const QString& reactionCode,
									const quint64& user);
		QDiscordRoute removeAllReactions(const quint64& channel,
										 const quint64& message);
		QDiscordRoute getReactionUsers(const quint64& channel,
									   const quint64& message,
									   const QString& reactionCode,
									   int limit);

		QDiscordRoute getMessageHistory(const quint64& channel,
										int limit);
		QDiscordRoute getMessageHistoryBefore(const quint64& channel,
											  int limit,
											  const QString& before);
		QDiscordRoute getMessageHistoryAfter(const quint64& channel,
											 int limit,
											 const QString& after);
		QDiscordRoute getMessageHistoryAround(const quint64& channel,
											  int limit,
											  const QString& around);

		//Bot-only
		QDiscordRoute getMessage(const quint64& channel,
								 const quint64& message);
		QDiscordRoute deleteMessages(const quint64& channel);
	}

	namespace Invites {
		QDiscordRoute getInvite(const QString& code);
		QDiscordRoute deleteInvite(const QString& code);
		QDiscordRoute getGuildInvites(const quint64& guild);
		QDiscordRoute getChannelInvites(const quint64& channel);
		QDiscordRoute createInvite(const quint64& channel);

		//Client-only
		QDiscordRoute acceptInvite(const QString& code);
	}
}

#endif // QDISCORDROUTE_HPP

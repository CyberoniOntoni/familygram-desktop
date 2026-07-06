/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "core/familygram_server.h"

#include "storage/localstorage.h"
#include "mtproto/mtproto_dc_options.h"
#include "logs.h"

#include <QtCore/QHostAddress>
#include <QtCore/QRegularExpression>

namespace FamilyGram {
namespace {
} // namespace

QString NormalizeServerHost(QString host) {
	host = host.trimmed().toLower();
	while (host.endsWith('/')) {
		host.chop(1);
	}
	const auto schemes = { u"https://"_q, u"http://"_q };
	for (const auto &scheme : schemes) {
		if (host.startsWith(scheme)) {
			host = host.mid(scheme.size());
			break;
		}
	}
	const auto slash = host.indexOf('/');
	if (slash >= 0) {
		host = host.left(slash);
	}
	const auto colon = host.indexOf(':');
	if (colon >= 0) {
		host = host.left(colon);
	}
	return host;
}

bool IsValidServerHost(const QString &host) {
	if (host.isEmpty() || host.size() > 253) {
		return false;
	}
	const auto address = QHostAddress(host);
	if (address.protocol() != QAbstractSocket::UnknownNetworkLayerProtocol) {
		return true;
	}
	static const auto RegExp = QRegularExpression(
		"^[a-z0-9]([a-z0-9.-]*[a-z0-9])?$");
	return RegExp.match(host).hasMatch();
}

void ApplyBootstrapDcOptions(not_null<MTP::DcOptions*> options) {
	options->constructFromBuiltIn();
}

} // namespace FamilyGram
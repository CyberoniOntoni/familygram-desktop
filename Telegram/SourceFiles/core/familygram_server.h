/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "base/bytes.h"

namespace MTP {
class DcOptions;
} // namespace MTP

namespace FamilyGram {

[[nodiscard]] QString NormalizeServerHost(QString host);
[[nodiscard]] bool IsValidServerHost(const QString &host);

constexpr int kBootstrapPortDc1 = 20443;
constexpr int kBootstrapPortDc2 = 20543;
constexpr int kBootstrapPortDc3 = 20643;

void ApplyBootstrapDcOptions(not_null<MTP::DcOptions*> options);

[[nodiscard]] bool HasConfiguredServer();
void EnsureBootstrapDcOptions(not_null<MTP::DcOptions*> options);

} // namespace FamilyGram
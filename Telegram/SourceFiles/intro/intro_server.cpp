/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "intro/intro_server.h"

#include "intro/intro_start.h"
#include "intro/intro_phone.h"
#include "intro/intro_qr.h"
#include "intro/intro_widget.h"
#include "core/familygram_server.h"
#include "storage/localstorage.h"
#include "main/main_account.h"
#include "ui/widgets/fields/input_field.h"
#include "styles/style_intro.h"

namespace Intro {
namespace details {

ServerWidget::ServerWidget(
	QWidget *parent,
	not_null<Main::Account*> account,
	not_null<Data*> data)
: Step(parent, account, data)
, _host(this, st::introPhone, rpl::single(
	u"e.g. 154.211.187.131 or family.example.com"_q)) {
	setTitleText(rpl::single(u"Connect to your server"_q));
	setDescriptionText(rpl::single(
		u"Enter the IP address or domain of your FamilyGram server. "
		u"The client will download the remaining connection settings "
		u"from the server."_q));

	const auto saved = Local::readFamilyGramServerHost();
	if (!saved.isEmpty()) {
		_host->setText(saved);
	}

	_host->submits() | rpl::on_next([=] {
		submit();
	}, _host->lifetime());
	_setFocus.events() | rpl::on_next([=] {
		_host->setFocusFast();
	}, lifetime());
}

void ServerWidget::submit() {
	const auto host = FamilyGram::NormalizeServerHost(_host->getLastText());
	if (!FamilyGram::IsValidServerHost(host)) {
		_host->setFocus();
		_host->showError();
		showError(rpl::single(
			u"Enter a valid server IP address or domain name."_q));
		return;
	}
	hideError();
	connectToServer(host);
}

void ServerWidget::connectToServer(const QString &host) {
	Local::writeFamilyGramServerHost(host);

	auto &mtp = account().mtp();
	FamilyGram::ApplyBootstrapDcOptions(&mtp.dcOptions());
	mtp.restart();
	apiClear();

	const auto weak = base::make_weak(this);
	_requestId = api().request(MTPhelp_GetConfig(
	)).done([=](const MTPConfig &result) {
		_requestId = 0;
		const auto &data = result.c_config();
		if (data.vdc_options().v.isEmpty()) {
			showError(rpl::single(
				u"The server returned an empty configuration."_q));
			return;
		}
		account().mtp().dcOptions().setFromList(data.vdc_options());
		if (const auto widget = weak.data()) {
			if (const auto intro = qobject_cast<Intro::Widget*>(
					widget->parentWidget())) {
				intro->requestNearestDc();
			}
			switch (getData()->afterServer) {
			case EnterPoint::Phone:
				widget->goNext<PhoneWidget>();
				break;
			case EnterPoint::Qr:
				widget->goNext<QrWidget>();
				break;
			default:
				widget->goNext<StartWidget>();
				break;
			}
		}
	}).fail([=](const MTP::Error &error) {
		_requestId = 0;
		const auto message = (error.type() == u"CONNECTION_NOT_INITED"_q)
			? u"Could not reach the server. Check the address, port "
			  u"forwarding (20443), and that the server is running."_q
			: u"Could not connect to the server: "_q + error.type();
		showError(rpl::single(message));
		_host->setFocus();
	}).send();
}

rpl::producer<QString> ServerWidget::nextButtonText() const {
	return rpl::single(u"Connect"_q);
}

void ServerWidget::resizeEvent(QResizeEvent *e) {
	Step::resizeEvent(e);
	_host->resizeToWidth(st::introNextButton.width);
	_host->moveToLeft(contentLeft(), contentTop() + st::introStepFieldTop);
}

void ServerWidget::activate() {
	Step::activate();
	setInnerFocus();
}

void ServerWidget::setInnerFocus() {
	_setFocus.fire({});
}

void ServerWidget::cancelled() {
	api().request(base::take(_requestId)).cancel();
}

} // namespace details
} // namespace Intro
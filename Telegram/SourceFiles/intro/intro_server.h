/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "intro/intro_step.h"

namespace Ui {
class InputField;
} // namespace Ui

namespace Intro {
namespace details {

class ServerWidget : public Step {
public:
	ServerWidget(
		QWidget *parent,
		not_null<Main::Account*> account,
		not_null<Data*> data);

	void submit() override;
	rpl::producer<QString> nextButtonText() const override;
	void activate() override;
	void setInnerFocus() override;
	void cancelled() override;

protected:
	void resizeEvent(QResizeEvent *e) override;

private:
	void connectToServer(const QString &host);

	object_ptr<Ui::InputField> _host;
	mtpRequestId _requestId = 0;
	rpl::event_stream<> _setFocus;

};

} // namespace details
} // namespace Intro
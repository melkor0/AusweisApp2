/*!
 * \copyright Copyright (c) 2014-2020 Governikus GmbH & Co. KG, Germany
 */

#include "SelfInformationWidget.h"
#include "ui_SelfInformationWidget.h"

#include "AppSettings.h"

#include <QDesktopServices>
#include <QPainter>
#include <QStyleOption>
#include <QUrl>

using namespace governikus;

SelfInformationWidget::SelfInformationWidget(QWidget* pParent)
	: QWidget(pParent)
	, mUi(new Ui::SelfInformationWidget())
{
	mUi->setupUi(this);

	connect(mUi->selfAuthenticationButton, &QAbstractButton::clicked, this, &SelfInformationWidget::selfAuthenticationRequested);
	const auto& generalSettings = Env::getSingleton<AppSettings>()->getGeneralSettings();
	connect(&generalSettings, &GeneralSettings::fireLanguageChanged, this, &SelfInformationWidget::onLanguageChanged);
	connect(&generalSettings, &GeneralSettings::fireDeveloperOptionsChanged, this, &SelfInformationWidget::onDeveloperOptionsChanged);

	mPixDescLogoLabel.reset(new QPixmap(QStringLiteral(":/images/siteWithLogo.png")));
	mUi->descriptionLogoLabel->setPixmap(mPixDescLogoLabel->scaled(159, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	onLanguageChanged();
	onDeveloperOptionsChanged();
}


SelfInformationWidget::~SelfInformationWidget()
{
}


void SelfInformationWidget::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void SelfInformationWidget::onLanguageChanged()
{
	const auto desc = tr("Use the button 'See my personal data now...' to display the data stored on your ID card. An Internet connection is required to display the data.");
	const auto hyperlink = QStringLiteral(R"(<a href="%1">%2</a>)").arg(tr("https://www.ausweisapp.bund.de/datenschutz/"), tr("data privacy statement"));
	const auto info = tr("Your personal data is neither saved nor processed in any way. Please see our %1 for details on how your personal data is processed.").arg(hyperlink);
	mUi->descriptionLabel->setText(desc + QStringLiteral("<br><br>") + info);
}


void SelfInformationWidget::onDeveloperOptionsChanged()
{
	if (Env::getSingleton<AppSettings>()->getGeneralSettings().useSelfAuthTestUri())
	{
		mUi->selfAuthenticationButton->setStyleSheet(QStringLiteral("QPushButton { background: red; }"));
		mUi->selfAuthenticationButton->setToolTip(tr("Test environment"));
	}
	else
	{
		mUi->selfAuthenticationButton->setStyleSheet(QString());
		mUi->selfAuthenticationButton->setToolTip(QString());
	}
}


void SelfInformationWidget::changeEvent(QEvent* pEvent)
{
	if (pEvent->type() == QEvent::LanguageChange)
	{
		mUi->retranslateUi(this);
		onLanguageChanged();
	}
	QWidget::changeEvent(pEvent);
}

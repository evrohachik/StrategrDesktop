//
// Created by Dmitry Khrykin on 2019-07-10.
//

#include <functional>

#include <QDebug>
#include <QPainter>
#include <QScrollBar>

#include "sessionsmainwidget.h"
#include "overviewwidget.h"
#include "strategysettingswidget.h"
#include "currentsessionwidget.h"
#include "slotboardwidget.h"
#include "notifierbackend.h"
#include "mainwindow.h"
#include "slotboardscrollarea.h"

SessionsMainWidget::SessionsMainWidget(QWidget *parent) : DataProviderWidget(parent) {
    auto *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, toolbarHeight(), 0, 0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);

    layoutChildWidgets();

    notifier.on_send_notification = [this](const stg::notification &notification) {
        std::cout << "want to sent notification: " << notification << "\n";
        notifierBackend.sendMessage(notification.title.c_str(),
                                    notification.message.c_str());
    };

    notifier.start_polling(ApplicationSettings::notifierTimerSecondsInterval);

    strategy().time_slots().add_on_ruler_change_callback([this] {
        strategySettingsWidget->reloadStrategy();
    });

    actionCenter().on_toggle_current_session = [this](bool currentSessionIsShown) {
        std::cout << "on_toggle_current_session: " << currentSessionIsShown << "\n";

        if (currentSessionIsShown) {
            currentSessionWidget->slideAndShow();
        } else {
            currentSessionWidget->slideAndHide();
        }
    };

    actionCenter().on_reload_current_session = [this]() {
        currentSessionWidget->reloadSessionIfNeeded();
    };

    currentSessionWidget->setVisible(actionCenter().current_session_is_shown());
    currentSessionWidget->reloadSessionIfNeeded();
}

SlotboardScrollArea *SessionsMainWidget::slotboardScrollArea() const {
    return _slotBoardScrollArea;
}

void SessionsMainWidget::toggleStrategySettingsOpen() {
    if (strategySettingsWidget->isHidden()) {
        strategySettingsWidget->slideAndShow();
    } else {
        strategySettingsWidget->slideAndHide();
    }
}

void SessionsMainWidget::layoutChildWidgets() {
    slotBoard = new SlotBoardWidget(this);

    _slotBoardScrollArea = new SlotboardScrollArea(this);
    _slotBoardScrollArea->setWidgetResizable(true);
    _slotBoardScrollArea->setFrameShape(QFrame::NoFrame);
    _slotBoardScrollArea->setWidget(slotBoard);

    strategySettingsWidget = new StrategySettingsWidget(this);
    strategySettingsWidget->hide();

    overviewWidget = new OverviewWidget(this);
    currentSessionWidget = new CurrentSessionWidget(this);

    auto *scrollAreaWindowWidget = new QWidget(this);
    scrollAreaWindowWidget->setObjectName("scrollAreaWindowWidget");
    scrollAreaWindowWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    scrollAreaWindowWidget->installEventFilter(this);

    connect(slotboardScrollArea()->verticalScrollBar(),
            &QScrollBar::valueChanged,
            [this](int value) { overviewWidget->update(); });

    layout()->addWidget(strategySettingsWidget);
    layout()->addWidget(overviewWidget);
    layout()->addWidget(currentSessionWidget);
    layout()->addWidget(scrollAreaWindowWidget);
}

void SessionsMainWidget::reloadStrategy() {
    slotBoard->reloadStrategy();
    strategySettingsWidget->reloadStrategy();
    overviewWidget->reloadStrategy();
    currentSessionWidget->reloadStrategy();

    notifier.schedule();
}

void SessionsMainWidget::paintEvent(QPaintEvent *paintEvent) {
    auto painter = QPainter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(baseColor());
    painter.drawRect(0, 0, width(), height());
}

void SessionsMainWidget::resizeEvent(QResizeEvent *event) {
    _slotBoardScrollArea->setGeometry(contentsRect());
}

bool SessionsMainWidget::eventFilter(QObject *object, QEvent *event) {
    if (object->objectName() == "scrollAreaWindowWidget" && event->type() == QEvent::Resize) {
        auto *scrollAreaWindowWidget = qobject_cast<QWidget *>(object);
        _slotBoardScrollArea->setViewportTopOffset(scrollAreaWindowWidget->geometry().top());
    }

    return false;
}
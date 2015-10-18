#include "config_radio.h"
#include "ui_config_radio.h"

#include "ts_helpers_qt.h"
#include <QPushButton>

ConfigRadio::ConfigRadio(QWidget *parent, QString title) :
    QDialog(parent), //ui(new Ui::ConfigRadio)
    m_title(title)
{
    if (title.isEmpty())  // global settings
    {
        ui = new Ui::ConfigRadio();
        ui->setupUi(this);
        this->setFixedSize(this->width(),this->height());
    }
    this->setAttribute( Qt::WA_DeleteOnClose );
}

ConfigRadio::~ConfigRadio()
{
    if (ui != nullptr)
        delete ui;
}

void ConfigRadio::UpdateEnabled(QString name, bool val)
{
    ConfigRadioGroupBox* channelStrip = GetChannelStrip(name);
    channelStrip->blockSignals(true);
    channelStrip->setChecked(val);
    channelStrip->blockSignals(false);
}

void ConfigRadio::UpdateBandpassInLowFrequency(QString name, double val)
{
    ConfigRadioGroupBox* channelStrip = GetChannelStrip(name);
    channelStrip->blockSignals(true);
    channelStrip->onInLoValueChanged(val);
    channelStrip->blockSignals(false);
}

void ConfigRadio::UpdateBandpassInHighFrequency(QString name, double val)
{
    ConfigRadioGroupBox* channelStrip = GetChannelStrip(name);
    channelStrip->blockSignals(true);
    channelStrip->onInHiValueChanged(val);
    channelStrip->blockSignals(false);
}

void ConfigRadio::UpdateDestruction(QString name, double val)
{
    ConfigRadioGroupBox* channelStrip = GetChannelStrip(name);
    channelStrip->blockSignals(true);
    channelStrip->onDestrValueChanged(val);
    channelStrip->blockSignals(false);
}

void ConfigRadio::UpdateRingModFrequency(QString name, double val)
{
    ConfigRadioGroupBox* channelStrip = GetChannelStrip(name);
    channelStrip->blockSignals(true);
    channelStrip->onRingModFrequencyValueChanged(val);
    channelStrip->blockSignals(false);
}

void ConfigRadio::UpdateRingModMix(QString name, double val)
{
    ConfigRadioGroupBox* channelStrip = GetChannelStrip(name);
    channelStrip->blockSignals(true);
    channelStrip->onRingModMixValueChanged(val);
    channelStrip->blockSignals(false);
}

void ConfigRadio::UpdateBandpassOutLowFrequency(QString name, double val)
{
    ConfigRadioGroupBox* channelStrip = GetChannelStrip(name);
    channelStrip->blockSignals(true);
    channelStrip->onOutLoValueChanged(val);
    channelStrip->blockSignals(false);
}

void ConfigRadio::UpdateBandpassOutHighFrequency(QString name, double val)
{
    ConfigRadioGroupBox* channelStrip = GetChannelStrip(name);
    channelStrip->blockSignals(true);
    channelStrip->onOutHiValueChanged(val);
    channelStrip->blockSignals(false);
}

ConfigRadioGroupBox* ConfigRadio::GetChannelStrip(QString name)
{
    if (m_ChannelStripMap.contains(name))
        return m_ChannelStripMap.value(name);

    ConfigRadioGroupBox* channelStrip = new ConfigRadioGroupBox(this);
    channelStrip->setObjectName(name);
    channelStrip->setTitle((m_title.isEmpty()) ? name : m_title);
    connect(channelStrip,SIGNAL(EnabledSet(QString,bool)),this,SIGNAL(EnabledSet(QString,bool)));
    connect(channelStrip,SIGNAL(InLoFreqSet(QString,double)),this,SIGNAL(InLoFreqSet(QString,double)));
    connect(channelStrip,SIGNAL(InHiFreqSet(QString,double)),this,SIGNAL(InHiFreqSet(QString,double)));
    connect(channelStrip,SIGNAL(DestructionSet(QString,double)),this,SIGNAL(DestructionSet(QString,double)));
    connect(channelStrip,SIGNAL(RingModFrequencySet(QString,double)),this,SIGNAL(RingModFrequencySet(QString,double)));
    connect(channelStrip,SIGNAL(RingModMixSet(QString,double)),this,SIGNAL(RingModMixSet(QString,double)));
    connect(channelStrip,SIGNAL(OutLoFreqSet(QString,double)),this,SIGNAL(OutLoFreqSet(QString,double)));
    connect(channelStrip,SIGNAL(OutHiFreqSet(QString,double)),this,SIGNAL(OutHiFreqSet(QString,double)));
    if (ui != nullptr)
        ui->horizontalLayout->addWidget(channelStrip);
    else
    {
        m_name = name;
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(channelStrip);
        QPushButton* remove_button = new QPushButton(this);
        QIcon remove_icon(":/icons/delete.png");
        remove_button->setIcon(remove_icon);
        remove_button->setAccessibleName("Delete");
        remove_button->setText("Delete");
        connect(remove_button, &QPushButton::clicked, [=] (bool checked)
        {
            Q_UNUSED(checked);
            // since the x button occupies Rejected, our bool means isDelete
            this->done(QDialog::DialogCode::Accepted);
        });
        layout->addWidget(remove_button, 0, Qt::AlignCenter);
        this->setLayout(layout);
        this->adjustSize();
        this->setFixedSize(this->width(),this->height());
        this->connect(this, &ConfigRadio::finished, [=](int r)
        {
            emit channel_closed(r, m_name); // emit in lambda, nice :)
        });
    }
    m_ChannelStripMap.insert(name,channelStrip);

    return channelStrip;
}

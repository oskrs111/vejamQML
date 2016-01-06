#ifndef QTKRTPCOMMAND_SETTINGS__H
#define QTKRTPCOMMAND_SETTINGS__H
#include <QObject>
#include "main.h"
#include "qtkRtpCommand_id.h"
#include "qtkRtpCommand_.h"

//This is a example template. Place this file and other qtkRtpCommand_xxxxx.h outside this folder ;-)...

class qtkParameterItem
{
public:
    enum pItemTypes
    {
        pitBinary = 0,
        pitNumber,
        pitString,
        pitOption
    };

    void setItem(QString name, QString category, QString value, QString csvOptions, quint8 type)
    {
        this->m_pName = name;
        this->m_pCategory = category;
        this->m_pValue = value;
        this->m_pCsvOptions = csvOptions;
        this->m_pType = type;
    }

    QString toJson()
    {
        return QString("{\"pName\":\"%1\", \"pCategory\":\"%2\", \"pValue\":\"%3\", \"pOptions\":\"%4\", \"pType\":\"%5\" }")
               .arg(this->m_pName)
               .arg(this->m_pCategory)
               .arg(this->m_pValue)
               .arg(this->m_pCsvOptions)
               .arg(this->m_pType,0,10);
    }

private:
    QString m_pName;
    QString m_pCategory;
    QString m_pValue;
    QString m_pCsvOptions;
    quint8 m_pType;
};

class qtkRtpCommand_Settings : public qtkRtpCommand_
{
    Q_OBJECT
public:
    explicit qtkRtpCommand_Settings(QtkJsRpcServer *parent = 0);

private:
    QtKApplicationParameters* p_params;
    void CommandInit();
    void CommandExecute(QJsonObject params, int seqId);
};

inline qtkRtpCommand_Settings::qtkRtpCommand_Settings(QtkJsRpcServer *parent)
        : qtkRtpCommand_(parent)
{
	this->CommandInit();
    this->p_params = __getApplicationParams();
}

inline void qtkRtpCommand_Settings::CommandInit()
{
    this->SetCommandId(k_rtp_command_id::rci_SettingsCommand);
}

inline void qtkRtpCommand_Settings::CommandExecute(QJsonObject params, int seqId)
//{"jsonrpc": "2.0", "method": "settings", "params":{"command":"getList"}, "id": 3}
{    
    QString result;
    QString data;
    QString name;
    QString category;
    QString value;

    if(this->p_params == 0)
    {
        result = QString("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32000, \"message\": \"Internal error. Data pointer is null!\"}, \"id\": %1}").arg(seqId,0,10);
        goto _abort;
    }

	//OSLL: Here will arrive just the method params as JSON object. So parse it and execute.
    //      Just chek it has the correct parameters
    if(params.contains("command"))
	{
        QString command = params.take("command").toString();
		if(command.compare(QString("getList")) == 0)
		{
            data.append(QString("["));
            qtkParameterItem pi;

            category = QString("aplicacion");
            name = QString("sync-interval");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("1;3600"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("streamming-id");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;8"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("streamming-alias");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("1;64"), qtkParameterItem::pitString);
            data.append(pi.toJson()).append(QString(","));

            category = QString("conexion");
            name = QString("mjpeg-port");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("1024;65000"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            category = QString("video");
            name = QString("scale-width");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("1;200"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("scale-height");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("1;200"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("scale-mode");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;1"), qtkParameterItem::pitOption);
            data.append(pi.toJson()).append(QString(","));

            name = QString("quality");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("-1;100"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("framerate-max");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("1;24"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("mirror-setting");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;3"), qtkParameterItem::pitOption);
            data.append(pi.toJson()).append(QString(","));

            name = QString("frame-drop");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;23"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("osd-show-title");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;1"), qtkParameterItem::pitBinary);
            data.append(pi.toJson()).append(QString(","));

            name = QString("osd-show-time");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;1"), qtkParameterItem::pitBinary);
            data.append(pi.toJson()).append(QString(","));

            name = QString("osd-x-position");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;1000"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("osd-y-position");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;1000"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("osd-text-font");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value,
                       QString("Helvetica;Times;Courier;OldEnglish;System;AnyStyle;Cursive;Monospace;Fantasy"),
                       qtkParameterItem::pitOption);
            data.append(pi.toJson()).append(QString(","));

            name = QString("osd-text-font-size");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("1;100"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("osd-text-font-weight");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("1;100"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("osd-text-font-color");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;16777215"), qtkParameterItem::pitNumber);
            data.append(pi.toJson()).append(QString(","));

            name = QString("device");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;1"), qtkParameterItem::pitOption);
            data.append(pi.toJson());

        #ifdef ANDROID_PLATFORM
            data.append(QString(","));
            category = QString("video-extras");
            name = QString("torch");
            value = this->p_params->loadParam(category, name,0);
            pi.setItem(name, category, value, QString("0;1"), qtkParameterItem::pitOption);
            data.append(pi.toJson());
        #endif

        data.append(QString("]"));
		}
        else
        {
            goto _commandNotFound;
        }
		
        result = QString("{\"jsonrpc\": \"2.0\", \"result\": \"success\", \"id\": %1, \"data\": %2}").arg(seqId,0,10).arg(data);
	}
	else
    {
_commandNotFound:
        result = QString("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32602, \"message\": \"Server error. Invalid method parameters\"}, \"id\": %1}").arg(seqId,0,10);
    }
_abort:
    emit commandDone(this->GetCommandId(), result.toLatin1());
}

#endif

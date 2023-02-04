# One time password (OTP) generator for Qt 

Works with both TOTP and HOTP with variable length.

## Installation

First clone this repository (or add it as a submodule if you use git):

- Standalone: `git clone https://github.com/RikudouSage/QtOneTimePassword.git otp`
- Submodule: `git submodule add https://github.com/RikudouSage/QtOneTimePassword.git otp`

The last argument is the target directory, `otp` in this case, it can be anything.

Then you add it to either your qmake `.pro` file or the your `CMakeLists.txt` (replace `otp` if you've cloned the repo in a different dir):

**qmake**:

```qmake
SOURCES += otp/onetimepasswordgenerator.cpp
HEADERS += otp/onetimepasswordgenerator.h
```

**cmake** - add the files to your `add_executable` instruction:
```CMakeLists
add_executable(your-app-name
  otp/onetimepasswordgenerator.cpp otp/onetimepasswordgenerator.h
  main.cpp
)
```

## Usage

The usage is pretty straightforward, but anyway here's an example:

```c++
#include <QCoreApplication>
#include <QDebug>

#include "otp/onetimepasswordgenerator.h"

QByteArray decodeBase32(const QString &input); // assume you have this function defined somewhere

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const auto secret = app.arguments().at(1); // take the base32 encoded string from the first argument as a secret

    OneTimePasswordGenerator totp;
    qDebug() << totp.generateHOTP(secret, 10, 6); // create a HOTP, the counter is set to 10, the digits count is set to 6
    qDebug() << totp.generateTOTP(secret, 6); // create a TOTP, the digits count is set to 6
    
    // using directly with base32 decoded raw bytes
    qDebug() << totp.generateHOTP(decodeBase32(secret), 10, 6);
    qDebug() << totp.generateTOTP(decodeBase32(secret), 6);

    return 0;
}
```

The string accepting functions are also invokable from QML:

```c++
// main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "otp/onetimepasswordgenerator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    // register the generator as a qml type within com.vendor
    qmlRegisterType<OneTimePasswordGenerator>("com.vendor", 1, 0, "OneTimePasswordGenerator");

    engine.load(url);

    return app.exec();
}

```

```qml
// main.qml
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

// import the registered C++ objects
import com.vendor 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("TOTP generator")

    property string totp
    property string hotp

    // initialize an OneTimePasswordGenerator and give it an id by which it can be accessed
    OneTimePasswordGenerator {
        id: otpGenerator
    }

    Column {
        width: parent.width
        spacing: 10
        TextField {
            id: totpSecret
            placeholderText: "Input the secret"
            width: parent.width
            onTextChanged: {
                // here you call the methods of the C++ OneTimePasswordGenerator
                hotp = otpGenerator.generateHOTP(text, hotpCounter.text || 0, 6);
                totp = otpGenerator.generateTOTP(text, 6);
            }
        }
        TextField {
            id: hotpCounter
            placeholderText: "Input the counter value (only for HOTP)"
            width: parent.width
            inputMethodHints: Qt.ImhDigitsOnly
        }

        Text {
            text: "TOTP: " + totp
        }
        Text {
            text: "HOTP: " + hotp
        }

        Button {
            text: qsTr("Regenerate")
            onClicked: {
                totpSecret.textChanged();
            }
        }
    }
}

```

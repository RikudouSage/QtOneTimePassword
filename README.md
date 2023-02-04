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
    QCoreApplication a(argc, argv);

    const auto secret = a.arguments().at(1); // take the base32 encoded string from the first argument as a secret

    OneTimePasswordGenerator totp;
    qDebug() << totp.generateHOTP(secret, 10, 6); // create a HOTP, the counter is set to 10, the digits count is set to 6
    qDebug() << totp.generateTOTP(secret, 6); // create a TOTP, the digits count is set to 6
    
    // using directly with base32 decoded raw bytes
    qDebug() << totp.generateHOTP(decodeBase32(secret), 10, 6);
    qDebug() << totp.generateTOTP(decodeBase32(secret), 6);

    return 0;
}
```

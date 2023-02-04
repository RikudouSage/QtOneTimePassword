# One time password (OTP) generator for Qt 

Works with both TOTP and HOTP with variable length.

## Usage

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
```CMakeList
add_executable(your-app-name
  otp/onetimepasswordgenerator.cpp otp/onetimepasswordgenerator.h
  main.cpp
)
```

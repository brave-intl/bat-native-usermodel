[![Build Status](https://travis-ci.org/brave-intl/bat-native-usermodel.svg?branch=master)](https://travis-ci.org/brave-intl/bat-native-usermodel)

# BAT Ads usermodels and machine learning

This library implements:
- Naive Bayes

## Installation

- Run `gn gen out\default` to generate ninja files
- Run `ninja -C out\default test` to compile
- Run `out\default\test.exe` for the tests
- Run `python scripts\cpplint.py` for linting

## Quick Standalone Testing Installation
- # after installing `brave-intl/bat-native-rapidjson` in parent dir
- `clang++ -std=c++17 -I./include -I./src -I../bat-native-rapidjson/include src/*.{c,cc} *.cc && ./a.out`

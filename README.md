[![Build Status](https://travis-ci.org/brave-intl/bat-native-usermodel.svg?branch=master)](https://travis-ci.org/brave-intl/bat-native-usermodel)

# BAT Ads usermodels and machine learning


This library implements:
- Naive Bayes
- Logistic Regression
- Feature extractions
- Page visitation callbacks and state update

## Installation

- Run `gn gen out\default` to generate ninja files
- Run `ninja -C out\default test` to compile
- Run `out\default\test.exe` for the tests
- Run `python scripts\cpplint.py` for linting
# BAT Native User Model

The user model contain an implementation of Naive Bayes and Logistic Regression.

The Naive Bayes fit uses multinomial distribution with a stopword list.

The Logistic Regression uses a feature vector and weights to return a
probability value.

The resulting data files are all log probabilities with 5 significant digits.

## Data

The `test/data` directory contains a test `model.json` file which uses the
standard English language stop word list.

A model file should look like this:

```
{
  "classes": [
    "class1", ...
  ],
  "priors": [
    lp1, ...
  ],
  "logProbs": {
    "word1": [lp1, ...],
    "word9": ...
  }
}
```

Consisting of a vector of class labels denoted by `"classes"`, a vector of prior
(log) class probabilities denoted by `"priors"` and a set of vectors of
multinomial log probabilities denoted by `"logProbs"` of porter-stemmed words
given the class.

The entries in the `"classes"` and `"priors"` arrays correspond to pairs, e.g.

```
"classes": [
  "red",
  "blue",
  "green"
],
"priors": [
  -1.0988, -1.0981, -1.0987
],
"logProbs": {
  "word1": [
    -10.4554,
    -10.2447,
    -9.5653,
  ],
  "word2": [
    -10.4554,
    -10.2447,
    -9.5653,
  ]
}
```

The class is implied by the ordering defined in `model.json`.

For example, given the `model.json` example above, each array in `model.json`
will have three values, with the first value corresponding to "red", the second
value corresponding to "blue", and the third value corresponding to "green".
Priors are "document frequencies" aka the probability of a class in the corpus
of documents.

For a better understanding of the text analysis approach, take a look at the
[quanteda](https://quanteda.io/) package.

### Taxonomies

The class space is flat; however a separation character (hyphen, "-") joins the
supercategory and subcategory, e.g., the "sports-rugby" class refers to a
supercategory of "sports" with a subcategory of "rugby".

## API

Initialize the User Model by calling `InitializePageClassifier(model, region)`
where `region` is a `std::string` containing the region of the user model and
`model` is a `std::string` containing the contents of the `model.json` file as
follows:

```
bool success = InitializePageClassifier(model, region)
```

Check if the page classifier has already been initialized by calling
`IsInitialized` as follows:

```
if (IsInitialized()) {
  // do something...
}
```

Classify a page by calling `ClassifyPage(html)` where `html` is a `std::string`
containing the pages HTML content, a vector of scores is returned as follows:

```
std::vector<double> scores = ClassifyPage(html);
```

Get the winning category by calling `GetWinningCategory(scores)` where `scores`
is a `std::vector<double>` of immediate or over time winning scores, the winning
category is returned as follows:

```
std::string winning_category = GetWinningCategory(scores);
```

## Unit Tests

```
npm run test -- brave_unit_tests --filter=UserModelTest.*
```

## Limitations

- BAT Native User Model should be run in a single thread in a subprocess.
- Log probabilities are encoded as negative numbers, future revisions may omit
  the minus-sign
- Consider implementing Porter Stemmer in C++

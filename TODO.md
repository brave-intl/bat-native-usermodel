# Plan:

Use as much as possible from current work to support a japanese language model. The model will rely on naive bayes as a classification algorithm, using hashed bag of substrings representation (see notebooks for a working reference in python). This will require some tweaks to support in the current infrastructure. 

## Required changes:

1. Model specification fields. Models should have a specification field (for example bag of words + linear model, or hashed representation + naive bayes), to know what the execution flow is. These should be put in the model .json file

2. An additional vectorizer / extractor should be added that supports hashed representations. It can adhere to the interface of the bag-of-words extractor, minimisng the ammount of effort to get a working implementation. 

## Testing: 

### Preliminaries: 
    - Pipeline produces reasonable results for japanese language text classification [POC notebook]()
    - Pipeline for languages other than japanese produces reasonable results (tested on english on the same notebook) 
    - C++ implementation of crc32 produces the same values with python. Introduces one new header file inclussion from [CRCpp](https://github.com/d-bahr/CRCpp) [Done]

### Software testing: 
    - Json representation of models can be updated to include versioning and representation fields.
    - Representation update does not break anything downstream. 

## Going forward:
This section outlines a number of steps for further in the future, which may be directly or indirectly related to this project. 

### Models
    - Acceptance criteria for new models
    - Automate testing to meet acceptance criteria (A CI for models)
    - New models (demographics etc)
    - New algorithms (for example [nb-svm](https://nlp.stanford.edu/pubs/sidaw12_simple_sentiment.pdf) should be a relatively easy next step to implement)

### Data
    - Increased and improved data gathering (add more data sources possibly in different native languages)
    - More nuanced annotation (a web page does not necessarily entail a single subject) 
    - More structured data sources, for example wikipedia can give a taxonomy of subject matter. This should help with the annotation aspects.

### Infrastructure
    - Multiple crawlers & Storage
    - Integrated testing environment for new models
    - Model propagation(?), ie how a new model may reach a browser.

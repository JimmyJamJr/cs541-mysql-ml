from transformers import TextClassificationPipeline, AutoModelForSequenceClassification, AutoTokenizer
model_path = "../lib/plugin/sentiment_model"

# Load the model and tokenizer directly from the local path
model = AutoModelForSequenceClassification.from_pretrained(model_path, local_files_only=True)
tokenizer = AutoTokenizer.from_pretrained(model_path, local_files_only=True)
sentiment_pipeline = TextClassificationPipeline(model=model, tokenizer=tokenizer)


def test():
    return "sentiment analysis code loaded"


def sentiment_analysis(data):
    final_results = []
    results = sentiment_pipeline(data)
    for res in results:
        if res['label'] == 'POSITIVE':
            return "Positive"
        else:
            return "Negative"


def sentiment_analysis_score(data):
    final_results = []
    results = sentiment_pipeline(data)
    for res in results:
        if res['label'] == 'POSITIVE':
            return str(res['score'])
        else:
            return str(res['score'])

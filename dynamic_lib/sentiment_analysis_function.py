from transformers import TextClassificationPipeline, AutoModelForSequenceClassification, AutoTokenizer
model_path = "sentiment_model"
# Load the model and tokenizer directly from the local path
model = AutoModelForSequenceClassification.from_pretrained(model_path, local_files_only=True)
tokenizer = AutoTokenizer.from_pretrained(model_path, local_files_only=True)
sentiment_pipeline = TextClassificationPipeline(model=model, tokenizer=tokenizer)
# import torch

def test():
    return "sentiment analysis code loaded"

def sentiment_analysis(data):
    final_results = []
    results = sentiment_pipeline(data)
    for res in results:
        if res['label'] == 'POSITIVE':
            #final_results.append({'sentiment': 'positive', 'score': res['score']})
            return "Positive " + str(res['score'])
        else:
            #final_results.append({'sentiment': 'negative', 'score': res['score']})
            return "Negative " + str(res['score'])

def sentiment_analysis(data):
    from transformers import TextClassificationPipeline, AutoModelForSequenceClassification, AutoTokenizer
    # Specify the local directory where the model and tokenizer are saved
    model_path = "sentiment_model"

    # Load the model and tokenizer directly from the local path
    model = AutoModelForSequenceClassification.from_pretrained(model_path, local_files_only=True)
    tokenizer = AutoTokenizer.from_pretrained(model_path, local_files_only=True)

    # Create Model
    sentiment_pipeline = TextClassificationPipeline(model=model, tokenizer=tokenizer)
    final_results = []
    
    results = sentiment_pipeline(data)
    for res in results:
        if res['label'] == 'POSITIVE':
            #final_results.append({'sentiment': 'positive', 'score': res['score']})
            final_results.append('positive')
        else:
            #final_results.append({'sentiment': 'negative', 'score': res['score']})
            final_results.append('negative')
    return final_results

#sentiment_analysis(["I love you", "I hate you"])
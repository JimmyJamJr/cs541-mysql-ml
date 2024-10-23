def sentiment_analysis(data):
    from transformers import pipeline
    final_results = []
    sentiment_pipeline = pipeline("sentiment-analysis")
    results = sentiment_pipeline(data)
    for res in results:
        if res['label'] == 'POSITIVE':
            #final_results.append({'sentiment': 'positive', 'score': res['score']})
            final_results.append('positive')
        else:
            #final_results.append({'sentiment': 'negative', 'score': res['score']})
            final_results.append('negative')
    return final_results
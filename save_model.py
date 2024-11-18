from transformers import pipeline
# Create Model
sentiment_pipeline = pipeline("sentiment-analysis")
# Save Model
# Replace "sentiment_model" with the name of the folder you want to save the model to
sentiment_pipeline.save_pretrained("sentiment_model")
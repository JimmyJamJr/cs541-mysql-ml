from transformers import pipeline
# Create Model
sentiment_pipeline = pipeline("sentiment-analysis", device=0)
# Save Model
# Replace "sentiment_model" with the name of the folder you want to save the model to
sentiment_pipeline.save_pretrained("sentiment_model")
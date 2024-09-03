from flask import Flask, request, render_template_string

app = Flask(__name__)

# Global variable to store the latest data
latest_data = {
    "personID": "N/A",
    "areaID": "N/A",
    "bpm": "N/A",
    "latitude": "N/A",
    "longitude": "N/A",
    "altitude": "N/A"
}

# Route to handle POST requests
@app.route('/verilerial', methods=['POST'])
def verilerial():
    global latest_data
    
    # Get data from POST request
    latest_data['personID'] = request.form.get('personID', "N/A")
    latest_data['areaID'] = request.form.get('areaID', "N/A")
    latest_data['bpm'] = request.form.get('bpm', "N/A")
    latest_data['latitude'] = request.form.get('latitude', "N/A")
    latest_data['longitude'] = request.form.get('longitude', "N/A")
    latest_data['altitude'] = request.form.get('altitude', "N/A")
    
    # Print to console for debugging
    print(latest_data)
    
    return "Data received", 200

# Route to display the latest data
@app.route('/')
def index():
    return render_template_string('''
        <h1>Latest Data</h1>
        <p><strong>Person ID:</strong> {{ personID }}</p>
        <p><strong>Area ID:</strong> {{ areaID }}</p>
        <p><strong>BPM:</strong> {{ bpm }}</p>
        <p><strong>Latitude:</strong> {{ latitude }}</p>
        <p><strong>Longitude:</strong> {{ longitude }}</p>
        <p><strong>Altitude:</strong> {{ altitude }}</p>
    ''', **latest_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

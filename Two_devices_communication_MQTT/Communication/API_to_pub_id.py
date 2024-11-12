from flask import Flask, request, jsonify

app = Flask(__name__)
pub_id = None


@app.route('/set_value', methods=['POST'])
def set_value():
    global pub_id
    data = request.get_json()
    pub_id = data.get("value")
    return jsonify({"status": "Value received", "value": pub_id}), 200


@app.route('/get_value', methods=['GET'])
def get_value():
    if pub_id is None:
        return jsonify({"status": "No value set"}), 404
    return jsonify({"value": pub_id}), 200

if __name__ == '__main__':
    app.run(port=5000)

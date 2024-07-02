var MQTT_CLIENT_ID = "browser_"+Math.floor((1 + Math.random()) * 0x10000000000).toString(16);

var MQTT_CLIENT = new Paho.MQTT.Client(secret.IP_ADDR, secret.PORT, MQTT_CLIENT_ID);

MQTT_CLIENT.connect({
    userName: secret.MQTT_USERNAME,
    password: secret.MQTT_PASS,
    onSuccess: function() {
        console.log("Connected to MQTT broker");
        MQTT_CLIENT.subscribe(secret.MQTT_TOPIC_1);
        MQTT_CLIENT.subscribe(secret.MQTT_TOPIC_2);
        MQTT_CLIENT.subscribe(secret.MQTT_TOPIC_3);
        MQTT_CLIENT.subscribe(secret.MQTT_TOPIC_4);
        MQTT_CLIENT.subscribe(secret.MQTT_TOPIC_5);
    },
    onFailure: function(errorMessage) {
        console.log("Failed to connect: ", errorMessage);
    }
});

MQTT_CLIENT.onConnectionLost = function(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("Connection lost:", responseObject.errorMessage);
    }
};

// This is the function which handles received messages
function myMessageArrived(message) {
    var topic = message.destinationName;
    var payload = message.payloadString;
    var messageDiv;
    
    if(topic == secret.MQTT_TOPIC_1) {
        messageDiv = document.getElementById("threshold_message");
        document.getElementById("watering_threshold").value = payload;
        document.getElementById("watering_threshold_slider").value = payload;
    }
    else if(topic == secret.MQTT_TOPIC_2) {
        messageDiv = document.getElementById("duration_message");
        document.getElementById("watering_duration").value = payload;
        document.getElementById("watering_duration_slider").value = payload;
    }
    else if(topic == secret.MQTT_TOPIC_3) {
        messageDiv = document.getElementById("repeat_message");
        document.getElementById("watering_repeat").value = payload;
        document.getElementById("watering_repeat_slider").value = payload;
    }
    else if(topic == secret.MQTT_TOPIC_4) {
        messageDiv = document.getElementById("soak_time_message");
        document.getElementById("watering_soak_time").value = parseInt(payload);
        document.getElementById("watering_soak_time_slider").value = parseInt(payload);
        document.getElementById("soakTimeUnit").value = payload.charAt(payload.length-1);
    }
    else if(topic == secret.MQTT_TOPIC_5) {
        messageDiv = document.getElementById("interval_message");
        document.getElementById("watering_interval").value = parseInt(payload);
        document.getElementById("watering_interval_slider").value = parseInt(payload);
        document.getElementById("intervalTimeUnit").value = payload.charAt(payload.length-1);
    }

    if(messageDiv) {
        messageDiv.innerHTML += "<p>"+payload+"</p>";
    }
};

// Tell MQTT_CLIENT to call myMessageArrived(message) each time a new message arrives
MQTT_CLIENT.onMessageArrived = myMessageArrived;

function publishMessage(topic) {
    var inputElement = document.getElementById(topic);
    var message = inputElement.value;

    if(topic == "watering_soak_time") {
        message += document.getElementById("soakTimeUnit").value;
    }
    else if(topic == "watering_interval") {
        message += document.getElementById("intervalTimeUnit").value;
    }

    var mqttMessage = new Paho.MQTT.Message(message);

    if(topic == "watering_threshold") mqttMessage.destinationName = secret.MQTT_TOPIC_1;
    else if(topic == "watering_duration") mqttMessage.destinationName = secret.MQTT_TOPIC_2;
    else if(topic == "watering_repeat") mqttMessage.destinationName = secret.MQTT_TOPIC_3;
    else if(topic == "watering_soak_time") mqttMessage.destinationName = secret.MQTT_TOPIC_4;
    else if(topic == "watering_interval") mqttMessage.destinationName = secret.MQTT_TOPIC_5;

    mqttMessage.retained = true;

    MQTT_CLIENT.send(mqttMessage);
    console.log("Message sent.");
}
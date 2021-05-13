const env_variables = {
    "fields1":"https://api.thingspeak.com/channels/1356067/fields/1.json?api_key=0NWJRGN2982F8X6U&results=1",
    "fields2":"https://api.thingspeak.com/channels/1356067/fields/2.json?api_key=0NWJRGN2982F8X6U&results=1",
    "fields3":"https://api.thingspeak.com/channels/1356067/fields/3.json?api_key=0NWJRGN2982F8X6U&results=1",
    "fields4":"https://api.thingspeak.com/channels/1356067/fields/4.json?api_key=0NWJRGN2982F8X6U&results=1"
}

function getEnvVariables(){
    return env_variables;
}
async function getWeatherData(url) {
    try {
        let myHeaders = new Headers();
        myHeaders.append("Content-Type", "application/json");
        let requestOptions = {
            method: 'GET',
            headers: myHeaders,
            redirect: 'follow'
        };
        const response = await fetch(url, requestOptions)
        return response.json();
    } catch (error) {
        return error.error;
    }
}

async function a() {
    let url = getEnvVariables();
    let field1_Data = await getWeatherData(url.fields1);
    let field2_Data = await getWeatherData(url.fields2);
    let field3_Data = await getWeatherData(url.fields3);
    let field4_Data = await getWeatherData(url.fields4);
    const date = new Date(field1_Data.feeds[0].created_at);
    const month = date.toLocaleString('default', { month: 'long' });
    const year = date.getFullYear();
    const currentDate = date.getDate();
    aa(field1_Data.feeds[0].field1, field2_Data.feeds[0].field2, field3_Data.feeds[0].field3, field4_Data.feeds[0].field4, month,year,currentDate)
}

a();

function aa(tempValue, humidityVal, pressVal, rainVal, monthName, year, date) {
    document.getElementsByClassName('tempValue')[0].innerText = Math.round(tempValue);
    document.getElementsByClassName('pressVal')[0].innerText = Math.round(pressVal) + " mb";
    document.getElementsByClassName('rainVal')[0].innerText = Math.round(rainVal) + "%";
    document.getElementsByClassName('humidityVal')[0].innerText = Math.round(humidityVal) + "%";
    document.getElementsByClassName('monthName')[0].innerText = monthName;
    document.getElementsByClassName('year')[0].innerText = year;
    document.getElementsByClassName('dateValue')[0].innerText = date;
}
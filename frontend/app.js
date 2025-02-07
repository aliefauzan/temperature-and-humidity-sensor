const API_URL = "xxxxxxxxxxxxxxxxx";

// Initialize Gauges
const temperatureGauge = new JustGage({
  id: "temperatureGauge",
  value: 0,
  min: 0,
  max: 100,
  title: "Temperature (°C)",
  gaugeWidthScale: 0.6,
  levelColors: ["#00ff00", "#ffcc00", "#ff0000"]
});

const humidityGauge = new JustGage({
  id: "humidityGauge",
  value: 0,
  min: 0,
  max: 100,
  title: "Humidity (%)",
  gaugeWidthScale: 0.6,
  levelColors: ["#00ff00", "#ffcc00", "#ff0000"]
});

// Initialize Charts
const temperatureChartCtx = document.getElementById("temperatureChart").getContext("2d");
const temperatureChart = new Chart(temperatureChartCtx, {
  type: "line",
  data: {
    labels: [],
    datasets: [{
      label: "Temperature (°C)",
      data: [],
      borderColor: "blue",
      borderWidth: 2,
      fill: false,
    }],
  },
  options: { responsive: true }
});

const humidityChartCtx = document.getElementById("humidityChart").getContext("2d");
const humidityChart = new Chart(humidityChartCtx, {
  type: "line",
  data: {
    labels: [],
    datasets: [{
      label: "Humidity (%)",
      data: [],
      borderColor: "green",
      borderWidth: 2,
      fill: false,
    }],
  },
  options: { responsive: true }
});

// Fetch data from backend
async function fetchData() {
  try {
    const response = await fetch(API_URL);
    const data = await response.json();
    return data[0]; // Assume backend returns an array with sensor data
  } catch (error) {
    console.error("Error fetching data:", error);
    return null;
  }
}

// Update UI
async function updateDashboard() {
  const sensorData = await fetchData();
  if (!sensorData) return;

  const currentTime = new Date().toLocaleTimeString();

  // Update Gauges
  temperatureGauge.refresh(sensorData.temperature_C);
  humidityGauge.refresh(sensorData.humidity);

  // Update Temperature Chart
  temperatureChart.data.labels.push(currentTime);
  temperatureChart.data.datasets[0].data.push(sensorData.temperature_C);
  if (temperatureChart.data.labels.length > 20) {
    temperatureChart.data.labels.shift();
    temperatureChart.data.datasets[0].data.shift();
  }
  temperatureChart.update();

  // Update Humidity Chart
  humidityChart.data.labels.push(currentTime);
  humidityChart.data.datasets[0].data.push(sensorData.humidity);
  if (humidityChart.data.labels.length > 20) {
    humidityChart.data.labels.shift();
    humidityChart.data.datasets[0].data.shift();
  }
  humidityChart.update();
}

// Refresh data every 2 seconds
setInterval(updateDashboard, 2000);

package model

import (
	"database/sql"
	"fmt"
	_ "github.com/go-sql-driver/mysql"
)

// Struktur untuk data sensor
type SensorData struct {
	ID             int     `json:"id"`
	Humidity       float64 `json:"humidity"`
	TemperatureC   float64 `json:"temperature_C"`
	TemperatureF   float64 `json:"temperature_F"`
}

var db *sql.DB

// Fungsi untuk menghubungkan ke database MySQL
func ConnectDB() error {
	var err error
	dsn := "root:xxxxxxx/sensor_db" // Kredensial MySQL
	db, err = sql.Open("mysql", dsn)
	if err != nil {
		return err
	}
	fmt.Println("DB Connected")
	return db.Ping()
}

// Fungsi untuk mendapatkan data sensor terbaru dari database
func GetSensorData() ([]SensorData, error) {
	rows, err := db.Query("SELECT id, humidity, temperature_C, temperature_F FROM sensor_readings ORDER BY id DESC LIMIT 1")
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	var data []SensorData
	for rows.Next() {
		var sensor SensorData
		if err := rows.Scan(&sensor.ID, &sensor.Humidity, &sensor.TemperatureC, &sensor.TemperatureF); err != nil {
			return nil, err
		}
		data = append(data, sensor)
	}
	return data, nil
}

// Fungsi untuk menyimpan data sensor ke database
func SaveSensorData(data SensorData) error {
	_, err := db.Exec("INSERT INTO sensor_readings (humidity, temperature_C, temperature_F) VALUES (?, ?, ?)", data.Humidity, data.TemperatureC, data.TemperatureF)
	if err != nil {
		return err
	}
	return nil
}

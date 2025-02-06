package model

import (
	"database/sql"

	_ "github.com/go-sql-driver/mysql" // Driver MySQL
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
	// Gantilah dengan kredensial MySQL Anda
	dsn := "root@tcp(localhost:3305)/sensor_db"
	db, err = sql.Open("mysql", dsn)
	if err != nil {
		return err
	}
	return db.Ping()
}

// Fungsi untuk mendapatkan data sensor terbaru dari database
func GetSensorData() ([]SensorData, error) {
	rows, err := db.Query("SELECT id, humidity, temperature_C, temperature_F FROM sensors ORDER BY id DESC LIMIT 1")
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
	_, err := db.Exec("INSERT INTO sensors (humidity, temperature_C, temperature_F) VALUES (?, ?, ?)", data.Humidity, data.TemperatureC, data.TemperatureF)
	if err != nil {
		return err
	}
	return nil
}

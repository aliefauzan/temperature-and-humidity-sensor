package handler

import (
	"encoding/json"
	"fmt"
	"net/http"
	"backend/src/model"
)

// Fungsi untuk mendapatkan data sensor dari database
func GetData(w http.ResponseWriter, r *http.Request) {
	data, err := model.GetSensorData()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	// Mengirimkan data dalam format JSON
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(data)
}

// Fungsi untuk menerima data sensor dan menyimpannya ke database
func UpdateData(w http.ResponseWriter, r *http.Request) {
	var data model.SensorData
	err := json.NewDecoder(r.Body).Decode(&data)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	err = model.SaveSensorData(data)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
	fmt.Fprintf(w, "Data received and stored successfully!")
}

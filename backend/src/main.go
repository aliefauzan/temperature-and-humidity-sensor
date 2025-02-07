package main

import (
	"fmt"
	"log"
	"net/http"
	"backend/src/handler"
	"backend/src/model"

	"github.com/gorilla/mux"
)

func main() {
	err := model.ConnectDB()
	if err != nil {
		log.Fatal("Failed to connect to database:", err)
	}

	// Setup router
	r := mux.NewRouter()
	r.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintln(w, "Hello, World!")
	}).Methods("GET")
	
	r.HandleFunc("/data", handler.GetData).Methods("GET")
	r.HandleFunc("/update", handler.UpdateData).Methods("POST")

	// Menjalankan server
	fmt.Println("Server started at http://localhost:8080")
	log.Fatal(http.ListenAndServe(":8080", r))
}

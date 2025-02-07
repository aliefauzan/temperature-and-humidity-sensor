package main

import (
	"fmt"
	"log"
	"net/http"
	"backend/src/handler"
	"backend/src/model"

	"github.com/gorilla/mux"
)

func enableCORS(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type, Authorization")
		if r.Method == "OPTIONS" {
			w.WriteHeader(http.StatusOK)
			return
		}
		next.ServeHTTP(w, r)
	})
}

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
	r.Use(enableCORS)
	serverAddr := ":8080"
	fmt.Printf("Server started at http://localhost%s\n", serverAddr)
	log.Fatal(http.ListenAndServe(serverAddr, r))
}

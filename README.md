Petrol Pump Management System

This project is a simple Petrol Pump Management System written in C.
It allows the user to record daily fuel sales, view the sales made on the current date, and calculate the total fuel sold.
All records are stored in a CSV file so that the data remains saved even after the program is closed.

Features

Add a new sale (vehicle number, driver name, fuel type, liters, total amount)

View all sales made today

View total liters of Petrol, Diesel, and Other fuels sold today

Data is stored in a sales.csv file for later viewing

How the Program Works

The system mainly uses the following functions:

make_sale() – Takes user input and stores a sale entry

view_sales_today() – Reads the CSV file and shows only the sales of the current date

totals_today() – Calculates the total liters of each fuel type sold today

today_date() – Generates the current system date in YYYY-MM-DD format

The program compares the date saved in each row with the current date to filter today's sales.

How to Compile and Run

Make sure GCC is installed on your system.

Keep all files (main.c, sales.c, sales.h, and an empty sales.csv) in the same folder.

Open the terminal in that folder and compile using:

gcc main.c sales.c -o petrol


Run the program with:

./petrol

File Overview
main.c

Contains the menu and user interface.
It calls the functions defined in sales.c based on the user’s choice.

sales.c

Handles all logic:

Saving sales to the CSV file

Reading today’s sales

Calculating totals

Generating the next sale ID

sales.h

Contains the structure definition and function prototypes.

sales.csv

Stores sale records in CSV format. This file is created automatically when the first sale is made.

Notes

All fuel rates are predefined in the code for simplicity.

The program is intended as a basic academic project and does not include advanced validation or error handling.

Only simple single-word driver names are supported in this version.

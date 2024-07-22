# Online Ticketing System

Welcome to the Online Ticketing System project! This system allows clients to view available movies and book tickets through a shared memory space between server-client programs. The server and client communicate to ensure ticket availability, process reservations, and handle multiple clients simultaneously using semaphores for synchronization.

## Features

- Display available movies and showtimes.
- Book multiple tickets for a selected movie.
- Check ticket availability before processing the reservation.
- Calculate the total bill for the booked tickets.
- Confirm or cancel the reservation based on user input.
- Update the movie ticket information and save booking records.
- Handle simultaneous clients with synchronized ticket decrements.

## Project Structure

- `server.c`: Server program that handles client requests and manages ticket reservations.
- `client.c`: Client program that allows users to view and book tickets.
- `movieTicketsInfo.txt`: File containing movie ticket information.
- `BookingRecord.txt`: File storing the booking records.
- `Makefile`: Makefile to compile the server and client programs.

## Files

### `movieTicketsInfo.txt`
This file contains information about available movies and the number of tickets. The format is as follows:
<MovieCount>
<MovieNumber>/<MovieName>/<TicketCount>
...
####Example:
5
1/Donkey King/500
2/Avengers/300
3/Bohemian Rhapsody/300
4/Halloween/400
5/First Man/500

### `BookingRecord.txt`
<RecordCount>
<CNIC>/<MovieNumber>/<ReservedTickets>
...
####Example:
4
1990/2/4
6667/4/4
7865/4/6
5849/2/3

/*
# Online Ticketing System

## Usage

1. Compile the server and client programs using the provided Makefile:
   ```bash
   make
2. Run the server program:
   ```bash
   ./server
3. Run the client program:
   ```bash
   ./client

## Functionality

### Server
- Displays a welcome message and available movies to the client.
- Processes the client's movie and ticket selection.
- Checks ticket availability and calculates the total bill.
- Confirms the reservation and updates the ticket information.
- Saves the booking record and handles errors appropriately.
- Uses semaphores to ensure synchronized access to ticket decrement functionality.

### Client
- Receives and displays the welcome message and available movies.
- Sends movie selection and number of tickets to the server.
- Displays the total bill and prompts the user to confirm or cancel the reservation.
- Sends CNIC, movie number, and reserved tickets to the server upon confirmation.
- Displays the reservation status message.

## Synchronization

Semaphores are used to synchronize access to the shared memory space, ensuring that only one client can decrement the ticket count at a time.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

## Acknowledgments

Thanks to everyone who contributed to this project and helped make it a success.

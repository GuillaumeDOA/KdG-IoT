-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: db
-- Generation Time: Oct 20, 2024 at 11:49 AM
-- Server version: 10.6.19-MariaDB-ubu2004
-- PHP Version: 8.2.24

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `iot_management`
--

-- --------------------------------------------------------

--
-- Table structure for table `customers`
--

CREATE TABLE `customers` (
  `id` int(11) NOT NULL,
  `first_name` varchar(255) NOT NULL,
  `last_name` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `phone_numer` varchar(255) NOT NULL,
  `address` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `customers`
--

INSERT INTO `customers` (`id`, `first_name`, `last_name`, `email`, `phone_numer`, `address`) VALUES
(1, 'John', 'Doe', 'john.doe@example.com', '1234567890', 'Breydelstraat 28, 2018 Antwerpen'),
(2, 'Jane', 'Smith', 'jane.smith@example.com', '0987654321', 'De Keyserlei 59, 2000 Antwerpen'),
(3, 'Alice', 'Johnson', 'alice.johnson@example.com', '1122334455', 'Bredabaan 811, 2170 Merksem'),
(4, 'Bob', 'Brown', 'bob.brown@example.com', '6677889900', 'Vogelzangstraat 29, 2560 Nijlen'),
(5, 'Charlie', 'Davis', 'charlie.davis@example.com', '2233445566', 'Everdongenlaan 15/6-7, 2300 Turnhout ');

-- --------------------------------------------------------

--
-- Table structure for table `devices`
--

CREATE TABLE `devices` (
  `id` int(11) NOT NULL,
  `device_type` varchar(255) DEFAULT NULL,
  `serial_number` varchar(255) DEFAULT NULL,
  `installation_date` date DEFAULT NULL,
  `customer_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `devices`
--

INSERT INTO `devices` (`id`, `device_type`, `serial_number`, `installation_date`, `customer_id`) VALUES
(1, 'Thermostat', 'SN123456', '2024-01-15', 1),
(2, 'Smart Lock', 'SN654321', '2024-02-10', 2),
(3, 'Security Camera', 'SN789012', '2024-03-05', 3),
(4, 'Light Bulb', 'SN345678', '2024-04-12', 4),
(5, 'Roomba', 'SN987654', '2024-05-20', 5);

-- --------------------------------------------------------

--
-- Table structure for table `maintenance_logs`
--

CREATE TABLE `maintenance_logs` (
  `id` int(11) NOT NULL,
  `device_id` int(11) DEFAULT NULL,
  `technician_name` varchar(255) DEFAULT NULL,
  `log_date` date DEFAULT NULL,
  `notes` text DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `maintenance_logs`
--

INSERT INTO `maintenance_logs` (`id`, `device_id`, `technician_name`, `log_date`, `notes`) VALUES
(1, 1, 'Technician A', '2024-01-30', 'Replaced the faulty sensor'),
(2, 2, 'Technician B', '2024-08-15', 'Checked for connectivity issues'),
(3, 3, 'Technician C', '2024-09-20', 'Updated firmware to the latest version'),
(4, 4, 'Technician B', '2024-10-05', 'Repaired wiring and connections'),
(5, 5, 'Technician C', '2024-10-12', 'Cleaned and recalibrated the device');

-- --------------------------------------------------------

--
-- Table structure for table `sensors`
--

CREATE TABLE `sensors` (
  `id` int(11) NOT NULL,
  `sensor_type` varchar(255) DEFAULT NULL,
  `device_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `sensors`
--

INSERT INTO `sensors` (`id`, `sensor_type`, `device_id`) VALUES
(1, 'Temperature', 1),
(2, 'Motion', 2),
(3, 'Camera', 3),
(4, 'Light', 4),
(5, 'Motion', 4),
(6, 'Motion', 5),
(7, 'Lidar', 5);

-- --------------------------------------------------------

--
-- Table structure for table `sensor_readings`
--

CREATE TABLE `sensor_readings` (
  `id` int(11) NOT NULL,
  `sensor_id` int(11) DEFAULT NULL,
  `reading_value` varchar(255) DEFAULT NULL,
  `reading_timestamp` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `sensor_readings`
--

INSERT INTO `sensor_readings` (`id`, `sensor_id`, `reading_value`, `reading_timestamp`) VALUES
(1, 1, '21°C', '2024-10-01 10:00:00'),
(2, 1, '20.4°C', '2024-10-01 11:00:00'),
(3, 2, 'Motion Detected', '2024-10-01 11:00:00'),
(4, 2, 'No Motion', '2024-10-01 12:00:00'),
(5, 3, 'Active', '2024-10-01 12:00:00'),
(6, 3, 'Inactive', '2024-10-01 13:00:00'),
(7, 4, 'Light', '2024-10-01 13:00:00'),
(8, 4, 'No Light', '2024-10-01 14:00:00'),
(9, 5, 'Motion Detected', '2024-10-01 11:00:00'),
(10, 5, 'No Motion', '2024-10-01 12:00:00'),
(11, 6, 'No Motion', '2024-10-01 11:00:00'),
(12, 6, 'No Motion', '2024-10-01 12:00:00'),
(13, 7, 'Object Detected', '2024-10-01 14:00:00'),
(14, 7, 'No Object Detected', '2024-10-01 11:00:00'),
(15, 1, '22.1°C', '2024-10-20 11:15:47'),
(17, 1, '24°C', '2024-10-20 11:44:36'),
(18, 1, '19°C', '2024-10-20 11:45:38'),
(19, 1, '23°C', '2024-10-20 11:46:14');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `customers`
--
ALTER TABLE `customers`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `email` (`email`);

--
-- Indexes for table `devices`
--
ALTER TABLE `devices`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `serial_number` (`serial_number`),
  ADD KEY `customer_id` (`customer_id`);

--
-- Indexes for table `maintenance_logs`
--
ALTER TABLE `maintenance_logs`
  ADD PRIMARY KEY (`id`),
  ADD KEY `device_id` (`device_id`);

--
-- Indexes for table `sensors`
--
ALTER TABLE `sensors`
  ADD PRIMARY KEY (`id`),
  ADD KEY `device_id` (`device_id`);

--
-- Indexes for table `sensor_readings`
--
ALTER TABLE `sensor_readings`
  ADD PRIMARY KEY (`id`),
  ADD KEY `sensor_id` (`sensor_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `customers`
--
ALTER TABLE `customers`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `devices`
--
ALTER TABLE `devices`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `maintenance_logs`
--
ALTER TABLE `maintenance_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `sensors`
--
ALTER TABLE `sensors`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT for table `sensor_readings`
--
ALTER TABLE `sensor_readings`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `devices`
--
ALTER TABLE `devices`
  ADD CONSTRAINT `devices_ibfk_1` FOREIGN KEY (`customer_id`) REFERENCES `customers` (`id`);

--
-- Constraints for table `maintenance_logs`
--
ALTER TABLE `maintenance_logs`
  ADD CONSTRAINT `maintenance_logs_ibfk_1` FOREIGN KEY (`device_id`) REFERENCES `devices` (`id`);

--
-- Constraints for table `sensors`
--
ALTER TABLE `sensors`
  ADD CONSTRAINT `sensors_ibfk_1` FOREIGN KEY (`device_id`) REFERENCES `devices` (`id`);

--
-- Constraints for table `sensor_readings`
--
ALTER TABLE `sensor_readings`
  ADD CONSTRAINT `sensor_readings_ibfk_1` FOREIGN KEY (`sensor_id`) REFERENCES `sensors` (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

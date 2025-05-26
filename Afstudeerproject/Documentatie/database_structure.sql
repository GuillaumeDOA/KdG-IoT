/*M!999999\- enable the sandbox mode */
-- MariaDB dump 10.19  Distrib 10.11.11-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: SMART_LOCK
-- ------------------------------------------------------
-- Server version       10.11.11-MariaDB-0ubuntu0.24.04.2

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `AccessLogs`
--

DROP TABLE IF EXISTS `AccessLogs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `AccessLogs` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `AccessTime` datetime DEFAULT current_timestamp(),
  `TagID` int(11) NOT NULL,
  `DoorID` int(11) NOT NULL,
  `Granted` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `TagID` (`TagID`),
  KEY `Unlocks_ibfk_2` (`DoorID`),
  CONSTRAINT `AccessLogs_ibfk_1` FOREIGN KEY (`TagID`) REFERENCES `Tags` (`ID`) ON DELETE CASCADE,
  CONSTRAINT `AccessLogs_ibfk_2` FOREIGN KEY (`DoorID`) REFERENCES `Doors` (`ID`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=121 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `Doors`
--

DROP TABLE IF EXISTS `Doors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `Doors` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Name` varchar(100) NOT NULL,
  `CurrentState` varchar(50) DEFAULT 'Closed',
  PRIMARY KEY (`ID`),
  KEY `idx_name` (`Name`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `TagDoorAccess`
--

DROP TABLE IF EXISTS `TagDoorAccess`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `TagDoorAccess` (
  `TagID` int(11) NOT NULL,
  `DoorID` int(11) NOT NULL,
  PRIMARY KEY (`TagID`,`DoorID`),
  KEY `DoorID` (`DoorID`),
  CONSTRAINT `TagDoorAccess_ibfk_1` FOREIGN KEY (`TagID`) REFERENCES `Tags` (`ID`) ON DELETE CASCADE,
  CONSTRAINT `TagDoorAccess_ibfk_2` FOREIGN KEY (`DoorID`) REFERENCES `Doors` (`ID`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `Tags`
--

DROP TABLE IF EXISTS `Tags`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `Tags` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `UserID` int(11) DEFAULT NULL,
  `Secret` varchar(255) NOT NULL,
  `Name` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `unique_user_per_tag` (`UserID`),
  UNIQUE KEY `unique_name_per_tag` (`Name`),
  KEY `UserID` (`UserID`),
  CONSTRAINT `Tags_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `Users` (`ID`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `Users`
--

DROP TABLE IF EXISTS `Users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8mb4 */;
CREATE TABLE `Users` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Firstname` varchar(50) NOT NULL,
  `Lastname` varchar(50) NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `unique_firstname_lastname` (`Firstname`,`Lastname`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-05-26  9:22:57
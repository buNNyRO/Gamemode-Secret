-- phpMyAdmin SQL Dump
-- version 5.0.1
-- https://www.phpmyadmin.net/
--
-- Gazdă: 127.0.0.1
-- Timp de generare: iul. 23, 2020 la 12:05 PM
-- Versiune server: 10.4.11-MariaDB
-- Versiune PHP: 7.4.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Bază de date: `suntnr1`
--

-- --------------------------------------------------------

--
-- Structură tabel pentru tabel `businesses`
--

CREATE TABLE `businesses` (
  `id` int(11) NOT NULL,
  `owner` varchar(24) NOT NULL DEFAULT '''AdmBot''',
  `model` int(11) NOT NULL,
  `price` int(11) NOT NULL,
  `eX` float NOT NULL,
  `eY` float NOT NULL,
  `eZ` float NOT NULL,
  `iX` float NOT NULL,
  `iY` float NOT NULL,
  `iZ` float NOT NULL,
  `interior` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Structură tabel pentru tabel `dmv_cp`
--

CREATE TABLE `dmv_cp` (
  `id` int(3) NOT NULL,
  `posx` float NOT NULL,
  `posy` float NOT NULL,
  `posz` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Eliminarea datelor din tabel `dmv_cp`
--

INSERT INTO `dmv_cp` (`id`, `posx`, `posy`, `posz`) VALUES
(2, 2810.97, -1849.7, 10.7456),
(3, 2820.72, -1882.87, 10.9774),
(4, 2746.25, -1888.31, 10.8828),
(5, 2645.41, -1805.39, 10.7636),
(6, 2645.05, -1687.83, 10.7498),
(7, 2659.66, -1659.68, 10.6997),
(8, 2789.99, -1659.48, 10.8158),
(9, 2839.33, -1659.37, 10.692),
(10, 2842, -1702.41, 11.0311),
(11, 2831.52, -1756.43, 10.875),
(12, 2823.19, -1818.78, 10.875),
(13, 2801.08, -1848.64, 9.88709);

-- --------------------------------------------------------

--
-- Structură tabel pentru tabel `users`
--

CREATE TABLE `users` (
  `id` int(8) NOT NULL,
  `name` varchar(32) NOT NULL,
  `password` varchar(65) NOT NULL,
  `remember_token` text NOT NULL DEFAULT '\'\'',
  `ip` varchar(18) NOT NULL,
  `mail` varchar(64) NOT NULL,
  `gender` int(2) NOT NULL DEFAULT 0,
  `admin` int(2) NOT NULL DEFAULT 0,
  `helper` int(2) NOT NULL DEFAULT 0,
  `activitystatus` int(2) NOT NULL DEFAULT 0,
  `skin` int(4) NOT NULL DEFAULT 0,
  `licenses` varchar(48) NOT NULL DEFAULT '''0|0|0|0|0|0|0|0''',
  `money` int(32) NOT NULL DEFAULT 0,
  `bankmoney` int(32) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Indexuri pentru tabele eliminate
--

--
-- Indexuri pentru tabele `businesses`
--
ALTER TABLE `businesses`
  ADD PRIMARY KEY (`id`);

--
-- Indexuri pentru tabele `dmv_cp`
--
ALTER TABLE `dmv_cp`
  ADD PRIMARY KEY (`id`);

--
-- Indexuri pentru tabele `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT pentru tabele eliminate
--

--
-- AUTO_INCREMENT pentru tabele `businesses`
--
ALTER TABLE `businesses`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pentru tabele `dmv_cp`
--
ALTER TABLE `dmv_cp`
  MODIFY `id` int(3) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=14;

--
-- AUTO_INCREMENT pentru tabele `users`
--
ALTER TABLE `users`
  MODIFY `id` int(8) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

-- phpMyAdmin SQL Dump
-- version 5.0.4
-- https://www.phpmyadmin.net/
--
-- Gazdă: 127.0.0.1
-- Timp de generare: nov. 30, 2021 la 02:43 PM
-- Versiune server: 10.4.17-MariaDB
-- Versiune PHP: 8.0.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Bază de date: `secret`
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

--
-- Eliminarea datelor din tabel `businesses`
--

INSERT INTO `businesses` (`id`, `owner`, `model`, `price`, `eX`, `eY`, `eZ`, `iX`, `iY`, `iZ`, `interior`) VALUES
(1, 'AdmBot', 1, 1000000, 1678.8, 1383.66, 10.741, 2306, -16, 27, 0),
(2, 'AdmBot', 8, 1000000, 1685.34, 1371.95, 10.7279, 363.218, -74.9702, 1001.51, 10);

-- --------------------------------------------------------

--
-- Structură tabel pentru tabel `cars`
--

CREATE TABLE `cars` (
  `id` int(11) NOT NULL,
  `model` int(11) NOT NULL,
  `owner` varchar(36) NOT NULL DEFAULT 'AdmBot',
  `user` int(11) NOT NULL DEFAULT 0,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `a` float NOT NULL,
  `color1` int(11) NOT NULL DEFAULT 1,
  `color2` int(11) NOT NULL DEFAULT 1,
  `ins_price` int(11) NOT NULL DEFAULT 0,
  `ins` int(11) NOT NULL DEFAULT 5,
  `km` float NOT NULL DEFAULT 0,
  `age` int(11) NOT NULL DEFAULT 0,
  `gas` int(11) NOT NULL DEFAULT 100,
  `plate` varchar(36) NOT NULL DEFAULT 'AdmBot'
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Eliminarea datelor din tabel `cars`
--

INSERT INTO `cars` (`id`, `model`, `owner`, `user`, `x`, `y`, `z`, `a`, `color1`, `color2`, `ins_price`, `ins`, `km`, `age`, `gas`, `plate`) VALUES
(6, 411, 'mr.bunny', 1, 1813.31, -1820.82, 12.9982, 0.991107, 0, 0, 27737, 0, 15.5196, 2, 59, '{CC0000}NewCar'),
(7, 541, 'mr.bunny', 1, 1826.62, -1817.92, 13.2017, 3.8072, 0, 0, 0, 5, 1.161, 2, 98, '{CC0000}NewCar'),
(8, 411, 'mr.bunny', 1, 2304.71, 1490.88, 42.816, 269.595, 0, 0, 0, 5, 0, 2, 100, '{CC0000}NewCar'),
(9, 560, 'mr.bunny', 1, 2301.99, 1437.19, 42.82, 269.595, 0, 0, 0, 5, 4.774, 2, 100, '{CC0000}NewCar'),
(10, 411, 'mr.bunny', 1, 2323.16, 1412.37, 35.824, 265.835, 0, 0, 0, 5, 0, 2, 100, '{CC0000}NewCar');

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
  `ip` varchar(18) NOT NULL,
  `mail` varchar(64) NOT NULL,
  `gender` int(2) NOT NULL DEFAULT 0,
  `admin` int(2) NOT NULL DEFAULT 0,
  `helper` int(2) NOT NULL DEFAULT 0,
  `activitystatus` int(2) NOT NULL DEFAULT 0,
  `skin` int(4) NOT NULL DEFAULT 223,
  `licenses` varchar(48) NOT NULL DEFAULT '''0|0|0|0|0|0|0|0''',
  `money` int(32) NOT NULL DEFAULT 0,
  `bankmoney` int(32) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Eliminarea datelor din tabel `users`
--

INSERT INTO `users` (`id`, `name`, `password`, `ip`, `mail`, `gender`, `admin`, `helper`, `activitystatus`, `skin`, `licenses`, `money`, `bankmoney`) VALUES
(1, 'mr.bunny', '1EC08200ABC3C97A272CA6CDED36D47C7B42FF853F359C70DA61701EE263CA2C', '192.168.8.104', 'vlogsdaniel13@gmail.com', 0, 6, 0, 0, 60, '\'0|0|0|0|0|0|0|0\'', 0, 0),
(2, 'Answer4k', '1EC08200ABC3C97A272CA6CDED36D47C7B42FF853F359C70DA61701EE263CA2C', '192.168.8.104', 'vlogsdaniel13@gmail.com', 0, 0, 0, 0, 223, '\'0|0|0|0|0|0|0|0\'', 0, 0);

--
-- Indexuri pentru tabele eliminate
--

--
-- Indexuri pentru tabele `businesses`
--
ALTER TABLE `businesses`
  ADD PRIMARY KEY (`id`);

--
-- Indexuri pentru tabele `cars`
--
ALTER TABLE `cars`
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
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT pentru tabele `cars`
--
ALTER TABLE `cars`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT pentru tabele `dmv_cp`
--
ALTER TABLE `dmv_cp`
  MODIFY `id` int(3) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=14;

--
-- AUTO_INCREMENT pentru tabele `users`
--
ALTER TABLE `users`
  MODIFY `id` int(8) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

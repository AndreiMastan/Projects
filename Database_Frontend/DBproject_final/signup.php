<?php
require 'includes/db.php';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $name = $_POST['name'];
    $surname = $_POST['surname'];
    $gender = $_POST['gender'];
    $residence = $_POST['residence'];
    $dob = $_POST['dob'];
    $phone = $_POST['phone'];
    $team = $_POST['team'];
    $password = password_hash($_POST['password'], PASSWORD_DEFAULT);

    // Insert into Users
    $stmt = $conn->prepare("INSERT INTO Users (Name, Surname, Gender, Residence, PasswordHash) VALUES (?, ?, ?, ?, ?)");
    $stmt->bind_param("sssss", $name, $surname, $gender, $residence, $password);
    $stmt->execute();
    $user_id = $stmt->insert_id;

    // Insert into UserDetails
    $stmt2 = $conn->prepare("INSERT INTO UserDetails (UserID, DateOfBirth, Phone, FavoriteTeam) VALUES (?, ?, ?, ?)");
    $stmt2->bind_param("isss", $user_id, $dob, $phone, $team);
    $stmt2->execute();

    header("Location: login.html");
    exit();
}
?>

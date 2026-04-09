<?php
require 'includes/db.php';
session_start();
if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}
$user_id = $_SESSION['user_id'];
$conn->query("INSERT INTO Orders (UserID) VALUES ($user_id)");
$order_id = $conn->insert_id;

$result = $conn->query("SELECT b.ProductID, b.Quantity FROM BasketItems b
                        JOIN Basket bk ON b.BasketID = bk.BasketID
                        WHERE bk.UserID = $user_id");

while ($row = $result->fetch_assoc()) {
    $pid = $row['ProductID'];
    $qty = $row['Quantity'];
    $conn->query("INSERT INTO OrderItems (OrderID, ProductID, Quantity) VALUES ($order_id, $pid, $qty)");
}

$conn->query("DELETE FROM BasketItems WHERE BasketID = (SELECT BasketID FROM Basket WHERE UserID = $user_id)");
header("Location: myorders.php");
?>
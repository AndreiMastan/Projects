<?php
require 'includes/db.php';
session_start();

if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}

$user_id = $_SESSION['user_id'];
$product_id = isset($_POST['product_id']) ? intval($_POST['product_id']) : 0;
$quantity = isset($_POST['quantity']) ? intval($_POST['quantity']) : 1;

// Check if product exists
$product_check = $conn->prepare("SELECT ProductID FROM Products WHERE ProductID = ?");
$product_check->bind_param("i", $product_id);
$product_check->execute();
$product_result = $product_check->get_result();

if ($product_result->num_rows === 0) {
    die("Invalid product ID.");
}

// Ensure basket exists
$basket_result = $conn->query("SELECT BasketID FROM Basket WHERE UserID = $user_id");
if ($basket_result->num_rows === 0) {
    $conn->query("INSERT INTO Basket (UserID) VALUES ($user_id)");
    $basket_id = $conn->insert_id;
} else {
    $basket = $basket_result->fetch_assoc();
    $basket_id = $basket['BasketID'];
}

// Add to basket
$stmt = $conn->prepare("INSERT INTO BasketItems (BasketID, ProductID, Quantity)
                        VALUES (?, ?, ?)
                        ON DUPLICATE KEY UPDATE Quantity = Quantity + ?");
$stmt->bind_param("iiii", $basket_id, $product_id, $quantity, $quantity);
$stmt->execute();

// Redirect to basket
header("Location: basket.php");
exit();
?>

<?php
require 'includes/db.php';
session_start();

if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}

$user_id = $_SESSION['user_id'];

$result = $conn->query("SELECT b.BasketItemID, p.Name, b.Quantity, (p.Price * b.Quantity) AS Subtotal
                        FROM BasketItems b
                        JOIN Basket bk ON b.BasketID = bk.BasketID
                        JOIN Products p ON b.ProductID = p.ProductID
                        WHERE bk.UserID = $user_id");

$total = 0;
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Basket</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body class="bg-light">
<div class="container mt-4">
    <h2 class="text-danger mb-4">Your Basket</h2>

    <?php if ($result->num_rows > 0): ?>
        <table class="table table-striped table-bordered align-middle">
            <thead class="table-dark">
                <tr>
                    <th>Name</th>
                    <th>Quantity</th>
                    <th>Subtotal</th>
                    <th>Action</th>
                </tr>
            </thead>
            <tbody>
            <?php while ($row = $result->fetch_assoc()): ?>
                <tr>
                    <td><?= $row['Name'] ?></td>
                    <td><?= $row['Quantity'] ?></td>
                    <td>$<?= $row['Subtotal'] ?></td>
                    <td>
                        <form method="post" action="remove_from_basket.php" class="d-inline">
                            <input type="hidden" name="item_id" value="<?= $row['BasketItemID'] ?>">
                            <button type="submit" class="btn btn-sm btn-danger">Remove</button>
                        </form>
                    </td>
                </tr>
                <?php $total += $row['Subtotal']; ?>
            <?php endwhile; ?>
            </tbody>
        </table>

        <p class="fw-bold">Total: $<?= $total ?></p>
        <a href="order.php" class="btn btn-success me-2">Proceed to Order</a>
        <a href="home.php" class="btn btn-secondary">Back to Store</a>

    <?php else: ?>
        <div class="alert alert-info">Your basket is currently empty.</div>
        <a href="home.php" class="btn btn-secondary">Go to Store</a>
    <?php endif; ?>
</div>
</body>
</html>

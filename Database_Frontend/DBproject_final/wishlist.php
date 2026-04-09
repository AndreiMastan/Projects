<?php
require 'includes/db.php';
session_start();

if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}

$user_id = $_SESSION['user_id'];

// Handle product removal
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['remove_id'])) {
    $remove_id = intval($_POST['remove_id']);
    $stmt = $conn->prepare("DELETE FROM Wishlist WHERE WishlistID = ? AND UserID = ?");
    $stmt->bind_param("ii", $remove_id, $user_id);
    $stmt->execute();
}

// Get wishlist items
$result = $conn->query("SELECT w.WishlistID, p.*
                        FROM Wishlist w
                        JOIN Products p ON w.ProductID = p.ProductID
                        WHERE w.UserID = $user_id");
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>My Wishlist</title>
    <!-- Bootstrap + Icons -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.5/font/bootstrap-icons.css" rel="stylesheet">
</head>
<body class="bg-light">
<div class="container mt-4">
    <h2 class="text-warning mb-4"><i class="bi bi-heart-fill"></i> My Wishlist</h2>

    <div class="mb-3">
        <a href="home.php" class="btn btn-outline-secondary">
            <i class="bi bi-house-door-fill me-1"></i> Back to Store
        </a>
    </div>

    <?php if ($result->num_rows > 0): ?>
        <div class="row">
            <?php while ($row = $result->fetch_assoc()): ?>
                <div class="col-md-3 col-sm-6 mb-4">
                    <div class="card h-100 shadow-sm">
                        <div class="text-center" style="height: 180px; overflow: hidden;">
                            <img src="<?= htmlspecialchars($row['ImagePath']) ?>" class="img-fluid p-2" alt="<?= htmlspecialchars($row['Name']) ?>" style="max-height: 100%; object-fit: contain;">
                        </div>
                        <div class="card-body d-flex flex-column">
                            <h5 class="card-title"><?= htmlspecialchars($row['Name']) ?></h5>
                            <p class="card-text">
                                <strong>Category:</strong> <?= htmlspecialchars($row['Category']) ?><br>
                                <strong>Price:</strong> $<?= $row['Price'] ?>
                            </p>
                            <a href="product.php?id=<?= $row['ProductID'] ?>" class="btn btn-primary w-100 mb-2">
                                View Product
                            </a>
                            <form method="post" class="d-grid">
                                <input type="hidden" name="remove_id" value="<?= $row['WishlistID'] ?>">
                                <button type="submit" class="btn btn-outline-danger">
                                    <i class="bi bi-trash3-fill me-1"></i> Remove
                                </button>
                            </form>
                        </div>
                    </div>
                </div>
            <?php endwhile; ?>
        </div>
    <?php else: ?>
        <div class="alert alert-info">Your wishlist is empty.</div>
    <?php endif; ?>
</div>
</body>
</html>

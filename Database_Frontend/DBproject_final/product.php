<?php
require 'includes/db.php';
session_start();

if (!isset($_SESSION['user_id'])) {
    header("Location: login.html");
    exit();
}

$user_id = $_SESSION['user_id'];

// Get and validate product ID
$id = isset($_GET['id']) ? intval($_GET['id']) : 0;
if ($id <= 0) {
    echo "Invalid product ID.";
    exit();
}

// Fetch product
$stmt = $conn->prepare("SELECT * FROM Products WHERE ProductID = ?");
$stmt->bind_param("i", $id);
$stmt->execute();
$result = $stmt->get_result();
$product = $result->fetch_assoc();
if (!$product) {
    echo "Product not found.";
    exit();
}

// Submit review
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['review_text'])) {
    $review_text = trim($_POST['review_text']);
    if (!empty($review_text)) {
        $stmt = $conn->prepare("INSERT INTO Reviews (ProductID, UserID, ReviewText) VALUES (?, ?, ?)");
        $stmt->bind_param("iis", $id, $user_id, $review_text);
        $stmt->execute();
    }
}

// Delete review
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['delete_review'])) {
    $review_id = intval($_POST['delete_review']);
    $stmt = $conn->prepare("DELETE FROM Reviews WHERE ReviewID = ? AND UserID = ?");
    $stmt->bind_param("ii", $review_id, $user_id);
    $stmt->execute();
}
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title><?= htmlspecialchars($product['Name']) ?> - Product</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body class="bg-light">
<div class="container mt-4">

    <a href="home.php" class="btn btn-secondary mb-3">← Back to Store</a>

    <div class="card mb-4 shadow-sm">
        <div class="row g-0">
            <div class="col-md-4 d-flex align-items-center justify-content-center p-3">
                <img src="<?= htmlspecialchars($product['ImagePath']) ?>" class="img-fluid rounded" alt="<?= htmlspecialchars($product['Name']) ?>" style="max-height: 250px; object-fit: contain;">
            </div>
            <div class="col-md-8">
                <div class="card-body">
                    <h3 class="card-title text-danger"><?= htmlspecialchars($product['Name']) ?></h3>
                    <p class="card-text">
                        <strong>Category:</strong> <?= htmlspecialchars($product['Category']) ?><br>
                        <strong>Price:</strong> $<?= $product['Price'] ?><br>
                        <strong>Producer:</strong> <?= htmlspecialchars($product['Producer']) ?><br>

                        <?php if ($product['Category'] === 'Balls'): ?>
                            <strong>Material:</strong> <?= htmlspecialchars($product['Material']) ?><br>
                            <strong>Durability:</strong> <?= htmlspecialchars($product['Durability']) ?>
                        <?php else: ?>
                            <strong>Size:</strong> <?= htmlspecialchars($product['Size']) ?><br>
                            <strong>Material:</strong> <?= htmlspecialchars($product['Material']) ?>
                        <?php endif; ?>
                    </p>

                    <!-- Add to Basket Form -->
                    <form method="post" action="add_to_basket.php" class="mt-3">
                        <input type="hidden" name="product_id" value="<?= $product['ProductID'] ?>">
                        <div class="mb-2">
                            <label for="quantity" class="form-label">Quantity:</label>
                            <input type="number" name="quantity" id="quantity" value="1" min="1" class="form-control" style="max-width: 100px;">
                        </div>
                        <button type="submit" class="btn btn-primary">Add to Basket</button>
                    </form>

                    <!-- Add to Wishlist Form -->
                    <form method="post" action="add_to_wishlist.php" class="mt-2">
                        <input type="hidden" name="product_id" value="<?= $product['ProductID'] ?>">
                        <button type="submit" class="btn btn-outline-warning">Add to Wishlist</button>
                    </form>

                </div>
            </div>
        </div>
    </div>

    <!-- Review Form -->
    <div class="card mb-4 shadow-sm">
        <div class="card-body">
            <h4 class="card-title text-success">Leave a Review</h4>
            <form method="post">
                <div class="mb-3">
                    <textarea name="review_text" class="form-control" rows="3" placeholder="Write your thoughts here..." required></textarea>
                </div>
                <button type="submit" class="btn btn-outline-success">Submit Review</button>
            </form>
        </div>
    </div>

    <!-- Reviews -->
    <div class="card shadow-sm mb-5">
        <div class="card-body">
            <h4 class="card-title text-primary">Reviews</h4>
            <?php
            $review_stmt = $conn->prepare("SELECT r.ReviewID, u.Name, r.ReviewText, r.ReviewDate, r.UserID
                                           FROM Reviews r
                                           JOIN Users u ON r.UserID = u.UserID
                                           WHERE r.ProductID = ?
                                           ORDER BY r.ReviewDate DESC");
            $review_stmt->bind_param("i", $id);
            $review_stmt->execute();
            $review_result = $review_stmt->get_result();

            if ($review_result->num_rows > 0): ?>
                <ul class="list-group">
                    <?php while ($review = $review_result->fetch_assoc()): ?>
                        <li class="list-group-item">
                            <div class="d-flex justify-content-between align-items-start">
                                <div>
                                    <strong><?= htmlspecialchars($review['Name']) ?>:</strong><br>
                                    <?= nl2br(htmlspecialchars($review['ReviewText'])) ?><br>
                                    <small class="text-muted"><?= $review['ReviewDate'] ?></small>
                                </div>
                                <?php if ($review['UserID'] == $user_id): ?>
                                    <form method="post" class="ms-3">
                                        <input type="hidden" name="delete_review" value="<?= $review['ReviewID'] ?>">
                                        <button type="submit" class="btn btn-sm btn-outline-danger">Delete</button>
                                    </form>
                                <?php endif; ?>
                            </div>
                        </li>
                    <?php endwhile; ?>
                </ul>
            <?php else: ?>
                <p class="text-muted">No reviews yet. Be the first!</p>
            <?php endif; ?>
        </div>
    </div>

</div>
</body>
</html>

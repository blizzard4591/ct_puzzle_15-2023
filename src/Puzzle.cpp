#include "Puzzle.h"

#include <iostream>

bool Puzzle::isComplete() const {
	return m_pieces.size() == 9;
}

bool Puzzle::addPiece(PuzzlePiece const& piece) {
	if (isComplete()) {
		std::cerr << "Internal Error: Puzzle already complete?!" << std::endl;
		throw;
	}

	std::size_t const pos = m_pieces.size();
	std::size_t const w = pos % 3;
	std::size_t const h = pos / 3;
	if (w > 0) { // Has left neighbor
		if (!isCompatibleHorizontal(m_pieces.at(coordsToIndex(w - 1, h)), piece)) {
			return false;
		}
	}
	if (h > 0) {
		if (!isCompatibleVertical(m_pieces.at(coordsToIndex(w, h - 1)), piece)) {
			return false;
		}
	}

	m_pieces.push_back(piece);
	return true;
}

void Puzzle::removeLastPiece() {
	if (m_pieces.size() == 0) {
		std::cerr << "Internal Error: Puzzle already empty?!" << std::endl;
		throw;
	}
	m_pieces.pop_back();
}

QImage Puzzle::toImage() const {
	if (!isComplete()) {
		std::cerr << "Internal Error: Puzzle not complete?!" << std::endl;
		throw;
	}

	int const blockWidth = 250;
	int const borderWidth = 10;
	int const blockWidthWithBorder = blockWidth + borderWidth;

	int const totalWidthAndHeight = 3 * blockWidth + 2 * borderWidth;
	QImage image(totalWidthAndHeight, totalWidthAndHeight, QImage::Format_ARGB32_Premultiplied);
	image.fill(Qt::lightGray);

	QPainter painter(&image);
	painter.setPen(QColorConstants::Red);


	for (int w = 0; w < 3; ++w) {
		for (int h = 0; h < 3; ++h) {

			QTransform transform = QTransform::fromTranslate(blockWidthWithBorder * w, blockWidthWithBorder * h);
			painter.setWorldTransform(transform);

			m_pieces.at(coordsToIndex(w, h)).paint(painter, w, h, blockWidth);
		}
	}
	return image;
}

bool Puzzle::doesArrowPointUpwards() const {
	for (auto it = m_pieces.cbegin(); it != m_pieces.cend(); ++it) {
		if (it->isArrowPiece()) {
			return it->getSideIndexArrowPointsAt() == 0;
		}
	}

	// If no arrow exists, every arrow points up.
	return true;
}

std::unique_ptr<Puzzle> Puzzle::rotateLeft() const {
	if (!isComplete()) {
		std::cerr << "Internal Error: Puzzle not complete?!" << std::endl;
		throw;
	}

	std::unique_ptr<Puzzle> result = std::make_unique<Puzzle>();
	for (int w = 2; w >= 0; --w) {
		for (int h = 0; h < 3; ++h) {
			PuzzlePiece const& p = m_pieces.at(coordsToIndex(w, h));
			if (!result->addPiece(p.rotate(Orientation::Rotation_90))) {
				std::cerr << "Internal Error: Failed to rotate puzzle, piece not accepted?!" << std::endl;
				throw;
			}
		}
	}
	return result;
}

QString Puzzle::getSolutionText() const {
	if (!isComplete()) {
		std::cerr << "Internal Error: Puzzle not complete?!" << std::endl;
		throw;
	}

	// Find direction the arrow is pointing in
	std::size_t indexArrowPointsAt = std::numeric_limits<std::size_t>::max();
	for (int w = 0; w < 3; ++w) {
		for (int h = 0; h < 3; ++h) {
			PuzzlePiece const& p = m_pieces.at(coordsToIndex(w, h));
			if (p.isArrowPiece()) {
				indexArrowPointsAt = p.getSideIndexArrowPointsAt();
				break;
			}
		}
	}

	if (indexArrowPointsAt == std::numeric_limits<std::size_t>::max()) {
		return QString();
	}

	std::unique_ptr<Puzzle> rotatedPuzzle = std::make_unique<Puzzle>(*this);
	while (indexArrowPointsAt > 0) {
		rotatedPuzzle = rotatedPuzzle->rotateLeft();
		--indexArrowPointsAt;
	}

	QString result;
	for (int h = 0; h < 3; ++h) {
		for (int w = 0; w < 3; ++w) {
			PuzzlePiece const& p = rotatedPuzzle->m_pieces.at(coordsToIndex(w, h));
			result.append(static_cast<char>(p.getChars(indexArrowPointsAt).a)).append(static_cast<char>(p.getChars(indexArrowPointsAt).b));
		}
	}
	return result;
}

std::size_t Puzzle::coordsToIndex(std::size_t const& w, std::size_t const& h) {
	return h * 3 + w;
}

bool Puzzle::isCompatibleVertical(PuzzlePiece const& top, PuzzlePiece const& bottom) {
	return ((top.getSideType(2) == bottom.getSideType(0)) && (top.getIsFemale(2) != bottom.getIsFemale(0)));
}

bool Puzzle::isCompatibleHorizontal(PuzzlePiece const& left, PuzzlePiece const& right) {
	return ((left.getSideType(1) == right.getSideType(3)) && (left.getIsFemale(1) != right.getIsFemale(3)));
}

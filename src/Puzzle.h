#ifndef CTPUZZLE_PUZZLE_H_
#define CTPUZZLE_PUZZLE_H_

#include <vector>

#include "PuzzlePiece.h"

class Puzzle {
public:
	bool isComplete() const;
	bool addPiece(PuzzlePiece const& piece);
	void removeLastPiece();

	QImage toImage() const;
	QString getSolutionText() const;

	std::unique_ptr<Puzzle> rotateLeft() const;
	bool doesArrowPointUpwards() const;

	static std::size_t coordsToIndex(std::size_t const& w, std::size_t const& h);
private:
	static bool isCompatibleVertical(PuzzlePiece const& top, PuzzlePiece const& bottom);
	static bool isCompatibleHorizontal(PuzzlePiece const& left, PuzzlePiece const& right);

	std::vector<PuzzlePiece> m_pieces;
};


#endif

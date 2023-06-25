#ifndef CTPUZZLE_PUZZLEPIECE_H_
#define CTPUZZLE_PUZZLEPIECE_H_

#include <array>
#include <limits>

#include "Orientation.h"
#include "SideType.h"
#include "TwoChars.h"

#include <QPainter>

class PuzzlePiece {
public:
	PuzzlePiece(std::array<SideType, 4> const& sides, std::array<bool, 4> const& sidesAreFemale, std::array<TwoChars, 4> const& chars);

	PuzzlePiece rotate(Orientation const& orientation) const;
	
	SideType getSideType(std::size_t const& index) const;
	bool getIsFemale(std::size_t const& index) const;
	TwoChars getChars(std::size_t const& index) const;

	static bool calcIsArrowPiece(std::array<SideType, 4> const& sides, std::array<bool, 4> const& isFemale);

	bool isArrowPiece() const;
	std::size_t getSideIndexArrowPointsAt() const;

	void paint(QPainter& parentPainter, int w, int h, int width) const;
private:
	// Top, Right, Bottom, Left
	std::array<SideType, 4> const m_sides;
	std::array<bool, 4> const m_isFemale;
	std::array<TwoChars, 4> const m_chars;
	bool const m_isArrowPiece;
};

#endif

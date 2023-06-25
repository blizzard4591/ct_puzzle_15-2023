#include "PuzzlePiece.h"

#include <QImage>

PuzzlePiece::PuzzlePiece(std::array<SideType, 4> const& sides, std::array<bool, 4> const& sidesAreFemale, std::array<TwoChars, 4> const& chars) : m_sides(sides), m_isFemale(sidesAreFemale), m_chars(chars), m_isArrowPiece(calcIsArrowPiece(sides, sidesAreFemale)) {
	//
}

template<typename T>
std::array<T, 4> rotateForwardOnce(std::array<T, 4> const& a) {
	return {
		a[1],
		a[2],
		a[3],
		a[0]
	};
}

template<typename T>
std::array<T, 4> rotateForwardTwice(std::array<T, 4> const& a) {
	return {
		a[2],
		a[3],
		a[0],
		a[1]
	};
}

template<typename T>
std::array<T, 4> rotateForwardThrice(std::array<T, 4> const& a) {
	return {
		a[3],
		a[0],
		a[1],
		a[2]
	};
}

PuzzlePiece PuzzlePiece::rotate(Orientation const& orientation) const {
	if (orientation == Orientation::Rotation_0) {
		return PuzzlePiece(*this);
	} else if (orientation == Orientation::Rotation_90) {
		return PuzzlePiece(rotateForwardOnce(m_sides), rotateForwardOnce(m_isFemale), rotateForwardOnce(m_chars));
	} else if (orientation == Orientation::Rotation_180) {
		return PuzzlePiece(rotateForwardTwice(m_sides), rotateForwardTwice(m_isFemale), rotateForwardTwice(m_chars));
	} else if (orientation == Orientation::Rotation_270) {
		return PuzzlePiece(rotateForwardThrice(m_sides), rotateForwardThrice(m_isFemale), rotateForwardThrice(m_chars));
	} else {
		throw;
	}
}

SideType PuzzlePiece::getSideType(std::size_t const& index) const {
	return m_sides[index];
}

bool PuzzlePiece::getIsFemale(std::size_t const& index) const {
	return m_isFemale[index];
}

TwoChars PuzzlePiece::getChars(std::size_t const& index) const {
	return m_chars[index];
}

bool PuzzlePiece::calcIsArrowPiece(std::array<SideType, 4> const& sides, std::array<bool, 4> const& isFemale) {
	return (
		((sides[0] == SideType::DoveTail) && (sides[1] == SideType::DoveTail) && (sides[2] == SideType::Rectangle) && (sides[3] == SideType::Rectangle) &&
			(isFemale[0] == false) && (isFemale[1] == true) && (isFemale[2] == true) && (isFemale[3] == false)) ||
		((sides[1] == SideType::DoveTail) && (sides[2] == SideType::DoveTail) && (sides[3] == SideType::Rectangle) && (sides[0] == SideType::Rectangle) &&
			(isFemale[1] == false) && (isFemale[2] == true) && (isFemale[3] == true) && (isFemale[0] == false)) ||
		((sides[2] == SideType::DoveTail) && (sides[3] == SideType::DoveTail) && (sides[0] == SideType::Rectangle) && (sides[1] == SideType::Rectangle) &&
			(isFemale[2] == false) && (isFemale[3] == true) && (isFemale[0] == true) && (isFemale[1] == false)) ||
		((sides[3] == SideType::DoveTail) && (sides[0] == SideType::DoveTail) && (sides[1] == SideType::Rectangle) && (sides[2] == SideType::Rectangle) &&
			(isFemale[3] == false) && (isFemale[0] == true) && (isFemale[1] == true) && (isFemale[2] == false))
		);
}

bool PuzzlePiece::isArrowPiece() const {
	return m_isArrowPiece;
}

std::size_t PuzzlePiece::getSideIndexArrowPointsAt() const {
	if (!isArrowPiece()) {
		return std::numeric_limits<std::size_t>::max();
	}
	for (std::size_t index = 0; index < 4; ++index) {
		if (m_sides[index] == SideType::DoveTail && m_isFemale[index] == true) {
			return index;
		}
	}
	return std::numeric_limits<std::size_t>::max();
}

void PuzzlePiece::paint(QPainter& parentPainter, int w, int h, int width) const {
	QImage subImage(width, width, QImage::Format_ARGB32_Premultiplied);
	subImage.fill(Qt::white);
	QPainter* painter = new QPainter(&subImage);

	int const subWidth = width / 5;
	int const boxWidth = 3 * subWidth;
	int const fifthWidth = boxWidth / 5;

	int const totalWidth = 150 + 50 + 50;
	for (std::size_t i = 0; i < 4; ++i) {
		//painter->drawText(3 * subWidth, 1 * subWidth, subWidth, subWidth, Qt::AlignVCenter | Qt::AlignHCenter, QString("%1 %2").arg(QString::number(m_chars.at(i).a, 16)).arg(QString::number(m_chars.at(i).b, 16)));
		painter->drawText(3 * subWidth, 1 * subWidth, subWidth, subWidth, Qt::AlignVCenter | Qt::AlignHCenter, QString("%1 %2").arg((char)m_chars.at(i).a).arg((char)m_chars.at(i).b));

		float const heightFactor = 0.666;
		bool const isFemale = m_isFemale.at(i);
		int const fF = isFemale ? -1 : 1;
		int const hBase = subWidth;
		int const hTop = subWidth - fF * subWidth * heightFactor;
		if (m_sides.at(i) == SideType::DoveTail) {
			painter->drawLine(subWidth, hBase, subWidth + subWidth * 1.33, hBase);
			painter->drawLine(subWidth + subWidth * 1.33, hBase, subWidth + subWidth, hTop);
			painter->drawLine(subWidth + subWidth, hTop, subWidth + subWidth + subWidth, hTop);
			painter->drawLine(subWidth + subWidth + subWidth, hTop, subWidth + subWidth + subWidth * 0.66, hBase);
			painter->drawLine(subWidth + subWidth + subWidth * 0.66, hBase, subWidth * 4, hBase);

			if (m_isArrowPiece && isFemale) {
				// Arrow points up at this.
				int const centerX = totalWidth / 2;
				int const centerY = totalWidth / 2;
				int const length = 15;
				int const edgeLength = 5;
				painter->drawLine(centerX, centerY - length, centerX, centerY + length);
				painter->drawLine(centerX, centerY - length, centerX + edgeLength, centerY - length + edgeLength);
				painter->drawLine(centerX, centerY - length, centerX - edgeLength, centerY - length + edgeLength);
			}
		} else if (m_sides.at(i) == SideType::Rectangle) {
			painter->drawLine(1 * subWidth, hBase, 2 * subWidth, hBase);
			painter->drawLine(2 * subWidth, hBase, 2 * subWidth, hTop);
			painter->drawLine(2 * subWidth, hTop, 3 * subWidth, hTop);
			painter->drawLine(3 * subWidth, hTop, 3 * subWidth, hBase);
			painter->drawLine(3 * subWidth, hBase, 4 * subWidth, hBase);
		} else if (m_sides.at(i) == SideType::Triangle) {
			painter->drawLine(1 * subWidth, hBase, 2 * subWidth, hBase);
			painter->drawLine(2 * subWidth, hBase, 2.5 * subWidth, hTop);
			painter->drawLine(2.5 * subWidth, hTop, 3 * subWidth, hBase);
			painter->drawLine(3 * subWidth, hBase, 4 * subWidth, hBase);
		} else if (m_sides.at(i) == SideType::TwoRounds) {
			painter->drawLine(subWidth + 0 * fifthWidth, hBase, subWidth + 1 * fifthWidth, hBase);
			painter->drawLine(subWidth + 2 * fifthWidth, hBase, subWidth + 3 * fifthWidth, hBase);
			painter->drawLine(subWidth + 4 * fifthWidth, hBase, subWidth + 5 * fifthWidth, hBase);
			painter->drawArc(QRect(subWidth + 1 * fifthWidth, hTop + 0.5 * (hBase - hTop), fifthWidth, hBase - hTop), 16 * 0, fF * 16 * 180);
			painter->drawArc(QRect(subWidth + 3 * fifthWidth, hTop + 0.5 * (hBase - hTop), fifthWidth, hBase - hTop), 16 * 0, fF * 16 * 180);
		}
		QTransform t;
		t.translate(width / 2.0, width / 2.0);
		t.rotate(-90);
		delete painter;
		subImage = subImage.transformed(t, Qt::SmoothTransformation);
		painter = new QPainter(&subImage);
	}
	delete painter;

	parentPainter.drawImage(0, 0, subImage);
}

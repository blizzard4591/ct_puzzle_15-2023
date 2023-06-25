#include <array>
#include <chrono>
#include <iostream>
#include <list>
#include <string>

#include <QImage>
#include <QString>

#include <QGuiApplication>

#include "Puzzle.h"
#include "CaesarChiffre.h"


void iterate(std::list<PuzzlePiece> const& piecesLeft, std::unique_ptr<Puzzle>& puzzle, std::size_t& counterFailures, bool& foundSolution) {
	static const std::vector<Orientation> orientations = { Orientation::Rotation_0, Orientation::Rotation_90, Orientation::Rotation_180, Orientation::Rotation_270 };

	std::size_t piecesLeftIndex = 0;
	for (auto it = piecesLeft.begin(); it != piecesLeft.end(); ++it) {
		auto const basePiece = *it;
		for (auto orIt = orientations.cbegin(); orIt != orientations.cend(); ++orIt) {
			auto const rotatedPiece = basePiece.rotate(*orIt);

			if (puzzle->addPiece(rotatedPiece)) {
				std::list<PuzzlePiece> subPiecesLeft = piecesLeft;
				auto subPiecesLeftIt = subPiecesLeft.begin();
				std::advance(subPiecesLeftIt, piecesLeftIndex);
				subPiecesLeft.erase(subPiecesLeftIt);
				
				if (subPiecesLeft.size() == 0) {
					// Done. From here we can only find rotationally symmetric solutions.
					foundSolution = true;
					return;
				} else {
					iterate(subPiecesLeft, puzzle, counterFailures, foundSolution);
					if (foundSolution) {
						return;
					}
				}
				puzzle->removeLastPiece();
			} else {
				++counterFailures;
			}
		}
		++piecesLeftIndex;
	}
}

int main(int argc, char* argv[]) {
	QGuiApplication app(argc, argv);

	std::cout << "c't, Puzzle 15/2023" << std::endl;
	
	std::list<PuzzlePiece> piecesLeft = {
		PuzzlePiece({SideType::TwoRounds, SideType::TwoRounds, SideType::Triangle, SideType::Rectangle}, {false, true, true, false}, {TwoChars(0x66, 0x70), TwoChars(0x41, 0x51), TwoChars(0x72, 0x59), TwoChars(0x4e, 0x6f)}),
		PuzzlePiece({SideType::DoveTail, SideType::DoveTail, SideType::Rectangle, SideType::Rectangle}, {false, true, true, false}, {TwoChars(0x59, 0x59), TwoChars(0x66, 0x68), TwoChars(0x67, 0x54), TwoChars(0x67, 0x53)}),
		PuzzlePiece({SideType::DoveTail, SideType::TwoRounds, SideType::DoveTail, SideType::Rectangle}, {true, false, false, true}, {TwoChars(0x62, 0x72), TwoChars(0x63, 0x72), TwoChars(0x6e, 0x79), TwoChars(0x61, 0x2e)}),
		
		PuzzlePiece({SideType::Triangle, SideType::TwoRounds, SideType::TwoRounds, SideType::Rectangle}, {true, false, false, true}, {TwoChars(0x72, 0x7a), TwoChars(0x21, 0x51), TwoChars(0x49, 0x42), TwoChars(0x52, 0x48)}),
		PuzzlePiece({SideType::DoveTail, SideType::Triangle, SideType::Triangle, SideType::Rectangle}, {false, true, true, false}, {TwoChars(0x74, 0x72), TwoChars(0x41, 0x51), TwoChars(0x41, 0x62), TwoChars(0x41, 0x72)}),
		PuzzlePiece({SideType::Triangle, SideType::Triangle, SideType::Rectangle, SideType::DoveTail}, {true, false, false, true}, {TwoChars(0x72, 0x65), TwoChars(0x6e, 0x66), TwoChars(0x47, 0x6e), TwoChars(0x3a, 0x41)}),

		PuzzlePiece({SideType::DoveTail, SideType::TwoRounds, SideType::Rectangle, SideType::TwoRounds}, {false, true, true, false}, {TwoChars(0x75, 0x21), TwoChars(0x5a, 0x42), TwoChars(0x51, 0x76), TwoChars(0x54, 0x68)}),
		PuzzlePiece({SideType::Triangle, SideType::Triangle, SideType::DoveTail, SideType::DoveTail}, {false, true, true, false}, {TwoChars(0x31, 0x35), TwoChars(0x5a, 0x42), TwoChars(0x6e, 0x70), TwoChars(0x76, 0x61)}),
		PuzzlePiece({SideType::Rectangle, SideType::TwoRounds, SideType::Rectangle, SideType::Triangle}, {false, true, true, false}, {TwoChars(0x76, 0x61), TwoChars(0x61, 0x74), TwoChars(0x75, 0x67), TwoChars(0x56, 0x66)})
	};

	auto const beginTotal = std::chrono::steady_clock::now();

	std::unique_ptr<Puzzle> puzzle = std::make_unique<Puzzle>();
	std::size_t counterFailures = 0;
	bool foundSolution = false;
	iterate(piecesLeft, puzzle, counterFailures, foundSolution);
	if (!foundSolution) {
		std::cerr << "Error: Could not identify solution to puzzle?!" << std::endl;
		return -1;
	}

	auto const endIterate = std::chrono::steady_clock::now();
	std::cout << "Looked at " << counterFailures << " combinations in " << std::chrono::duration_cast<std::chrono::milliseconds>(endIterate - beginTotal).count() << "ms." << std::endl;

	auto const beginImage = std::chrono::steady_clock::now();
	// Rotate until the arrow points upwards.
	while (!puzzle->doesArrowPointUpwards()) {
		puzzle = puzzle->rotateLeft();
	}

	auto const image = puzzle->toImage();
	image.save(QString("solution.png"));
	QString const solutionText = puzzle->getSolutionText();
	std::cout << "Solution text (respecting the arrow): " << solutionText.toStdString() << std::endl;
	auto const endImage = std::chrono::steady_clock::now();
	std::cout << "Created image in " << std::chrono::duration_cast<std::chrono::milliseconds>(endImage - beginImage).count() << "ms." << std::endl;

	auto const beginCaesar = std::chrono::steady_clock::now();
	
	for (int shift = 1; shift < 26; ++shift) {
		std::cout << "Solution (shifted by " << shift << "): " << CaesarCipher::decrypt(solutionText, shift).toStdString() << std::endl;
	}

	auto const endTotal = std::chrono::steady_clock::now();
	std::cout << "Enumerated Caesar solutions in " << std::chrono::duration_cast<std::chrono::milliseconds>(endTotal - beginCaesar).count() << "ms." << std::endl;

	std::cout << "Done!" << std::endl;
	return 0;
}

#ifdef _MSC_VER
int __stdcall WinMain(struct HINSTANCE__* hInstance, struct HINSTANCE__* hPrevInstance, char* lpszCmdLine, int nCmdShow) {
	return main(__argc, __argv);
}

#endif

#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side s) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    side = s;
    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    (side == WHITE) ? board.doMove(opponentsMove, BLACK) : 
                      board.doMove(opponentsMove, WHITE);

    Move *move = heuristicMove();
    board.doMove(move, side);
    return move;
}

/** 
 * Returns a random possible move on the board
 */
Move *Player::randomMove() {
    int random, x, y;
    Move* move = new Move(0, 0); 

    if (!board.hasMoves(side)) return nullptr;

    while (true)
    {
        // Get a random value between 0 and 63 and convert to x and y
        random = rand() % 64;
        x = random % 8;
        y = random / 8;

        // Set the coordinates in move and return it if it's a valid move
        move->setX(x);
        move->setY(y);

        if (board.checkMove(move, side))
        {
            return move;
        }
    }
}

Move *Player::heuristicMove () {
    // Initialize scores array to give every position a score of zero
    int score[64] = {0}, max = 64, next = 64;

    Move* move = new Move(0, 0); 

    // Store the first position's score, which we have defined as the number
    // of tiles we would capture if we moved here (the checkMove function 
    // has also been altered to reflect this)
    score[0] = board.checkMove(move, side);

    // Do the same for every other position
    for (int i = 1; i < 64; ++i)
    {
        move->setX(i % 8);
        move->setY(i / 8);

        score[i] = board.checkMove(move, side);
    }

    // Multiply the spots adjacent to the corners by -3 to make sure we take 
    // these spots only if we have to (since capture of the corner by the 
    // other side is likely)
    score[1] *= -3;
    score[6] *= -3;
    score[8] *= -3;
    score[9] *= -3;
    score[14] *= -3;
    score[15] *= -3;
    score[48] *= -3;
    score[49] *= -3;
    score[54] *= -3;
    score[55] *= -3;
    score[57] *= -3;
    score[62] *= -3;

    // Multiply the spots in the corner by 3 to make them more likely to be 
    // taken
    score[0] *= 3;
    score[7] *= 3;
    score[56] *= 3;
    score[63] *= 3;

    // Also increase the score for the edge pieces, but by a smaller amount
    for (int y = 1; y < 7; y++)
    {
        score[8*y] *= 1.5;
        score[7 + 8*y] *= 1.5;
    }

    for (int x = 1; x < 7; x++)
    {
        score[x] *= 1.5;
        score[x + 8*7] *= 1.5;
    }

    // Find the first nonzero score
    for (int i = 0; i < 64; i++)
    {

        if (score[i] != 0)
        {
            max = i;
            next = i + 1;
            break;

        }
    }

    // Look at the rest of the scores to see if we have any that are larger
    // (and nonzero in case there are only negative scores)
    for (; next < 64; next++)
    {
        if (score[next] > score[max] && score[next] != 0)
        {
            max = next;
        }
    }

    // If this is true, max was never changed, so all scores are zero and no
    // moves are possible
    if (max == 64) return nullptr;

    // Otherwise, reset the move and return it
    move->setX(max % 8);
    move->setY(max / 8);

    return move;
}
#include "TransferFunction.h"
#include <algorithm>


TransferFunction::TransferFunction(LinearPiece function)
{
    _pieces.push_back(function);
}
TransferFunction::TransferFunction(std::vector<LinearPiece> pieces) 
    :
    _pieces(pieces)
{}

QColor TransferFunction::classify(float intensity) const
{
    QColor color {0, 0, 0, 0};
    for(std::vector<LinearPiece>::const_iterator it =  _pieces.begin(); it != _pieces.end(); it++)
    {
        color = it->apply(intensity);
        if(color.alpha() > 0)
        {
            return color;
        }
    }
    return color;
}

void TransferFunction::addPiece(LinearPiece function)
{
    _pieces.push_back(function);
}

std::vector<LinearPiece>& TransferFunction::getPieces()
{
    return _pieces;
}

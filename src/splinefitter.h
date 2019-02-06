#ifndef SPLINEFITTER_H
#define SPLINEFITTER_H

#include <qwt_spline.h>
#include <qwt_spline_local.h>
#include <qwt_spline_pleasing.h>
#include <qwt_spline_cubic.h>
#include <qwt_spline_basis.h>
#include <qwt_spline_parametrization.h>
#include <qwt_curve_fitter.h>

namespace adiscope {
class SplineFitter: public QwtCurveFitter
{
public:
    enum Mode
    {
	PChipSpline,
	AkimaSpline,
	CubicSpline,
	CardinalSpline,
	ParabolicBlendingSpline,
	PleasingSpline,
	BasisSpline
    };

    SplineFitter( Mode mode );

    ~SplineFitter();

    void setClosing( bool on );

    void setBoundaryCondition( const QString &condition );

    void setParametric( const QString &parameterType );

    virtual QPolygonF fitCurve( const QPolygonF &points ) const;

    virtual QPainterPath fitCurvePath( const QPolygonF &points ) const;

private:
    void setBoundaryConditions( int condition, double value = 0.0 );

    Mode d_mode;
    QwtSpline *d_spline;
};
}
#endif // SPLINEFITTER_H

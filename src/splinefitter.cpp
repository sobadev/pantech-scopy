#include "splinefitter.h"

using namespace adiscope;

SplineFitter::SplineFitter(SplineFitter::Mode mode):
	QwtCurveFitter( QwtCurveFitter::Path ),
	d_mode(mode),
	d_spline(nullptr)
{
	switch( mode )
	{
	case PleasingSpline:
	{
		d_spline = new QwtSplinePleasing();
		break;
	}
	case PChipSpline:
	{
		d_spline = new QwtSplineLocal( QwtSplineLocal::PChip );
		break;
	}
	case AkimaSpline:
	{
		d_spline = new QwtSplineLocal( QwtSplineLocal::Akima );
		break;
	}
	case CubicSpline:
	{
		d_spline = new QwtSplineCubic();
		break;
	}
	case CardinalSpline:
	{
		d_spline = new QwtSplineLocal( QwtSplineLocal::Cardinal );
		break;
	}
	case ParabolicBlendingSpline:
	{
		d_spline = new QwtSplineLocal( QwtSplineLocal::ParabolicBlending );
		break;
	}
	case BasisSpline:
	{
		d_spline = new QwtSplineBasis();
		break;
	}
	}
	if ( d_spline )
		d_spline->setParametrization( QwtSplineParametrization::ParameterX );
}

SplineFitter::~SplineFitter()
{
	delete d_spline;
}

void SplineFitter::setClosing(bool on)
{
	if ( d_spline == nullptr )
		return;

	d_spline->setBoundaryType(
				on ? QwtSpline::ClosedPolygon : QwtSpline::ConditionalBoundaries );
}

void SplineFitter::setBoundaryCondition(const QString &condition)
{
	QwtSplineC2 *splineC2 = dynamic_cast<QwtSplineC2 *>( d_spline );
	if ( splineC2 )
	{
		if ( condition == "Cubic Runout" )
		{
			setBoundaryConditions( QwtSplineC2::CubicRunout );
			return;
		}

		if ( condition == "Not a Knot" )
		{
			setBoundaryConditions( QwtSplineC2::NotAKnot );
			return;
		}
	}

	QwtSplineC1 *splineC1 = dynamic_cast<QwtSplineC1 *>( d_spline );
	if ( splineC1 )
	{
		if ( condition == "Linear Runout" )
		{
			setBoundaryConditions( QwtSpline::LinearRunout, 0.0 );
			return;
		}

		if ( condition == "Parabolic Runout" )
		{
			// Parabolic Runout means clamping the 3rd derivative to 0.0
			setBoundaryConditions( QwtSpline::Clamped3, 0.0 );
			return;
		}
	}

	// Natural
	setBoundaryConditions( QwtSplineC1::Clamped2, 0.0 );
}

void SplineFitter::setParametric(const QString &parameterType)
{
	QwtSplineParametrization::Type type = QwtSplineParametrization::ParameterX;

	if ( parameterType == "Uniform" )
	{
		type = QwtSplineParametrization::ParameterUniform;
	}
	else if ( parameterType == "Centripetral" )
	{
		type = QwtSplineParametrization::ParameterCentripetal;
	}
	else if ( parameterType == "Chordal" )
	{
		type = QwtSplineParametrization::ParameterChordal;
	}
	else if ( parameterType == "Manhattan" )
	{
		type = QwtSplineParametrization::ParameterManhattan;
	}

	d_spline->setParametrization( type );
}

QPolygonF SplineFitter::fitCurve(const QPolygonF &points) const
{
	return d_spline->polygon( points, 0.5 );
}

QPainterPath SplineFitter::fitCurvePath(const QPolygonF &points) const
{
	return d_spline->painterPath( points );
}

void SplineFitter::setBoundaryConditions(int condition, double value)
{
	if ( d_spline == nullptr )
		return;

	// always the same at both ends

	QwtSpline *spline = dynamic_cast<QwtSpline *>( d_spline );
	if ( spline )
	{
		spline->setBoundaryCondition( QwtSpline::AtBeginning, condition );
		spline->setBoundaryValue( QwtSpline::AtBeginning, value );

		spline->setBoundaryCondition( QwtSpline::AtEnd, condition );
		spline->setBoundaryValue( QwtSpline::AtEnd, value );
	}
}

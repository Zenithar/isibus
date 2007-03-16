#ifndef __SPRITES_H__
#define __SPRITES_H__

#include <QGraphicsPixmapItem>

#define ID_BUS_LEFT_TO_RIGHT           01
#define ID_BUS_RIGHT_TO_LEFT           02
#define ID_BUS_FRONT_TO_BACK           03
#define ID_BUS_BACK_TO_FRONT           04
#define ID_CASE_PLAIN			05

class IsiSprite : public QGraphicsPixmapItem
{
public:
    IsiSprite( QList<QPixmap>* frames, QGraphicsScene* scene )
        : QGraphicsPixmapItem( 0, scene ), mFrames(frames), mCurrentFrame(-1),
        mXVelocity(0), mYVelocity(0)
	{
		setFrame(0);
	}
	void setFrame(int frame)
	{
		if( frame != mCurrentFrame && !mFrames->isEmpty() )
		{
			mCurrentFrame = frame % mFrames->count();
			setPixmap( mFrames->at( mCurrentFrame ) );
		}
	}
	int frame() const { return mCurrentFrame; }
	int frameCount() const { return mFrames->count(); }
	void setVelocity(qreal xv, qreal yv) { mXVelocity = xv; mYVelocity = yv; }
	qreal xVelocity() const { return mXVelocity; }
	qreal yVelocity() const { return mYVelocity; }
	virtual void advance(int phase) 
    	{ 
		if(phase==0) return;
		
		moveBy( xVelocity(), yVelocity() ); 
	}
private:
	QList<QPixmap>* mFrames;
	int mCurrentFrame;
	qreal mXVelocity;
	qreal mYVelocity;
};

class Bus : public IsiSprite
{
public:
    Bus (QList<QPixmap> *frames, QGraphicsScene *scene, int t, int sk, int st) 
        : IsiSprite( frames, scene ), myType(t), skip(sk), cskip(sk), step(st)
        { }

    void nextFrame()
	{
	    if (cskip-- <= 0) {
		setFrame( (frame()+step+frameCount())%frameCount() );
		cskip = qAbs(skip);
	    }
	}

    virtual int type() const { return myType; }

private:
    int myType;
    int skip;

    int cskip;
    int step;
};


class plainCase : public IsiSprite
{
public:
	int x;
	int y;
	
	plainCase(QList<QPixmap> *frames, QGraphicsScene *scene, int coordx, int coordy): IsiSprite( frames, scene )
	{
		this->x = coordx;
		this->y = coordy;
	}
};

class RoadCase:  public plainCase
{
public:
	int direction;
	int idArret;
	int segment;
	int idRoad;
	
	
	RoadCase(QList<QPixmap> *frames, QGraphicsScene *scene, int x, int y, int dir, int idArr, int seg, int idR): plainCase( frames, scene, x, y )
	{
		direction = dir;
		idArret = idArr;
		segment = seg;
		idRoad = idR;
	}
};


#endif

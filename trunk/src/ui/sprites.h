#ifndef __SPRITES_H__
#define __SPRITES_H__

#include <QGraphicsPixmapItem>

#define ID_BUS_LEFT_TO_RIGHT           01
#define ID_BUS_RIGHT_TO_LEFT           02
#define ID_BUS_FRONT_TO_BACK           03
#define ID_BUS_BACK_TO_FRONT           04

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
    { if(phase==0) return; moveBy( mXVelocity, mYVelocity ); }
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

#endif
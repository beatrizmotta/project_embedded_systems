from fastapi import FastAPI, Request
from fastapi.exceptions import RequestValidationError
from fastapi.responses import JSONResponse
from pydantic import BaseModel, validator

app = FastAPI()

class Infraction(BaseModel):
    plate: str
    timestamp: str 
    recorded_speed: int 
    image_hash: str 

    @validator('plate') 
    def plate_not_be_empty(cls, value):
        if not value.strip():
            raise ValueError('Plate must not be empty.')
        return value

    @validator('recorded_speed')
    def speed_must_be_number(cls, value):
        try:
            float(value)
        except ValueError:
            raise ValueError('Speed must be a number.')
        return value



@app.exception_handler(RequestValidationError)
async def validation_exception_handler(request: Request, exc: RequestValidationError):
    # Extract first error's message (you can customize this logic as needed)
    error_msg = exc.errors()[0]['msg'] if exc.errors() else "Invalid input"
    
    return JSONResponse(
        status_code=422,
        content={
            "message": "Was not able to save successfully",
            "error": error_msg
        }
    )

@app.post("/notify-infraction")
def notify_infraction(infraction: Infraction):
    return {
        "message": 'Infraction was received and saved successfully.',
        "content": infraction
    }

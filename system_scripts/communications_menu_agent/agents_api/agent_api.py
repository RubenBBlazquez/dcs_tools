import os
from fastapi import FastAPI, UploadFile, HTTPException, Form
from dotenv import load_dotenv
from fastapi.responses import JSONResponse
from langchain.agents.openai_assistant.base import OpenAIAssistantRunnable, _get_openai_client

load_dotenv(dotenv_path=os.path.join(os.path.dirname(__file__), "conf", ".env"))
app = FastAPI()
OPENAI_API_KEY = os.getenv("OPENAI_API_KEY")

DCS_MENU_PARSER_ASSISTANT_ID = "asst_IumuFTCriwUHSEXOp9Br7sWn"
DCS_COMMUNICATION_STEPS_GENERATOR_ASSISTANT_ID = "asst_SYcgz99OGPnLZtRgmO2KgdqB"

@app.post("/dcs/get_menu_content_from_image")
async def send_image_message(
    user_message: str = Form(...),
    image: UploadFile = None
):
    try:
        agent = OpenAIAssistantRunnable(assistant_id=DCS_MENU_PARSER_ASSISTANT_ID)
        print(response)
        return JSONResponse(content={"assistant_reply": response})
    except Exception as e:
        raise
        raise HTTPException(status_code=500, detail=str(e))

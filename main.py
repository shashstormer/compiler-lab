from fastapi import FastAPI, Response

app = FastAPI()


@app.get("/compiler/lab.sh")
async def compiler_lab_setup():
    with open('setup.sh', 'r') as f:
        return Response(f.read())


if __name__ == "__main__":
    import uvicorn

    uvicorn.run(app, host="0.0.0.0", port=5073)

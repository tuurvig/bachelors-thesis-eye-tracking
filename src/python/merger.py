import OpenEXR
import Imath
import numpy as np
import os.path
from glob import glob


def EXRGetRedChannel(filepath):
    # set image pixel type to float 16-bit
    pt = Imath.PixelType(Imath.PixelType.HALF)
    # load EXR image from filepath
    exrFile = OpenEXR.InputFile(filepath)
    # extract red channel string od pixels of type float 16-bit from the image
    channel = exrFile.channel("R", pt)
    # transform the channel string to numpy array and return it
    return np.frombuffer(channel, dtype = np.float16)


def MergeTextureInSubfolder(folderName, texture):
    # get the specified texture file located in the same subfolder
    # of multiple participants
    everyFile = glob("*/" + folderName + "/" + texture)
    # weight constant
    weight = 1 / len(everyFile)

    # open one texture and extract an alpha channel and header.
    exrFile = OpenEXR.InputFile(everyFile[0])
    header = exrFile.header()
    alpha = exrFile.channel("A", Imath.PixelType(Imath.PixelType.HALF))

    dw = header['dataWindow']
    # height and width of the image
    shape = (dw.max.x - dw.min.x + 1, dw.max.y - dw.min.y + 1)
    # create a 1D numpy array size of the texture inititalised with float16 zeros    
    merger = np.zeros(shape[0]*shape[1], dtype = np.float16)

    # interate the same type of texture of multiple participants
    for textureFile in everyFile:
        # get numpy red channel of the texture
        red = EXRGetRedChannel(textureFile)
        # transform the texture by weight and add the result to merger buffer
        merger = merger + red*weight
        
    # convert the merger buffer back to string
    R = merger.astype(np.float16).tobytes()
    # create output filepath
    outFile = "resultantdata/" + folderName + "/" + texture
    print(outFile)
    # create new EXR output image with the same header as the first texture
    exrOut = OpenEXR.OutputFile(outFile, header)
    # write red channel to all three channels.
    exrOut.writePixels({'R': R, 'G': R, 'B': R, 'A': alpha})
    # complete the image
    exrOut.close()


def main():
    textureRoot = "participant1/Gaze/"
    # get unique name of the textures
    textures = [ x[len(textureRoot):] for x in glob(textureRoot + "*")]

    # merge every texture in Gaze and Camera subfolder
    for texture in textures:
        MergeTextureInSubfolder("Gaze", texture)
        MergeTextureInSubfolder("Camera", texture)


if __name__ == '__main__':
    main()
